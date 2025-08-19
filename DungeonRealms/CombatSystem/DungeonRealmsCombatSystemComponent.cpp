#include "CombatSystem/DungeonRealmsCombatSystemComponent.h"
#include "CombatSystem/DungeonRealmsCombatStatics.h"
#include "CombatSystem/DungeonRealmsAttackTracer.h"
#include "CombatSystem/DungeonRealmsCombatSystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/DungeonRealmsGameplayEffectContext.h"
#include "Characters/DungeonRealmsCharacter.h"
#include "DungeonRealmsGameplayTags.h"
#include "DungeonRealmsLogChannels.h"
#include "Kismet/KismetMathLibrary.h"

UDungeonRealmsCombatSystemComponent* UDungeonRealmsCombatSystemComponent::FindCombatSystemComponent(const AActor* Actor)
{
	if (IsValid(Actor))
	{
		if (const IDungeonRealmsCombatSystemInterface* CombatSystemInterface = Cast<const IDungeonRealmsCombatSystemInterface>(Actor))
		{
			return CombatSystemInterface->GetCombatSystemComponent();	
		}
		
		UDungeonRealmsCombatSystemComponent* CombatSystemComponent = Actor->FindComponentByClass<UDungeonRealmsCombatSystemComponent>();
		if (IsValid(CombatSystemComponent))
		{
			return CombatSystemComponent;
		}
	}
	return nullptr;
}

UDungeonRealmsCombatSystemComponent::UDungeonRealmsCombatSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDungeonRealmsCombatSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	ActiveAttackTracer = NewObject<UDungeonRealmsAttackTracer>(this, AttackTracerClass);
}

void UDungeonRealmsCombatSystemComponent::BeginAttackTrace(FName SocketName)
{
	ADungeonRealmsCharacter* OwningCharacter = GetOwner<ADungeonRealmsCharacter>();
	AActor* AttachedActor = OwningCharacter->GetAttachedActorFromSocket(SocketName);
	if (IsValid(AttachedActor))
	{
		UPrimitiveComponent* HitboxComponent = AttachedActor->FindComponentByTag<UPrimitiveComponent>(FName("Hitbox"));
		if (IsValid(HitboxComponent))
		{
			ActiveAttackTracer->BeginTrace(HitboxComponent);
		}
		else
		{
			UE_LOG(LogDungeonRealms, Error, TEXT("%s actor doesn't have component with hitbox tag."), *AttachedActor->GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogDungeonRealms, Error, TEXT("Attached actor not found to %s socket."), *SocketName.ToString());
	}
}

void UDungeonRealmsCombatSystemComponent::PerformAttackTrace()
{
	if (UDungeonRealmsCombatStatics::HasObstacleBetween(GetOwner(), ActiveAttackTracer->GetSourceActor()))
	{
		return;
	}
	const TArray<FHitResult> Hits = ActiveAttackTracer->PerformTrace();
	const TArray<FHitResult> HostileTargets = FilterToHostileTargets(Hits);
	ApplyHitEvents(HostileTargets);
}

TArray<FHitResult> UDungeonRealmsCombatSystemComponent::FilterToHostileTargets(const TArray<FHitResult>& Hits) const
{
	TArray<FHitResult> HostileTargets;
	for (const FHitResult Hit : Hits)
	{
		const AActor* HitActor = Hit.GetActor();
		if (IsValid(HitActor) && UDungeonRealmsCombatStatics::HasHostileAttitude(GetOwner(),HitActor))
		{
			HostileTargets.Add(Hit);
		}
	}
	return HostileTargets;
}

void UDungeonRealmsCombatSystemComponent::ApplyHitEvents(const TArray<FHitResult>& Hits) const
{
	TArray<TWeakObjectPtr<AActor>> HitTargets;
	
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (const UDungeonRealmsCombatSystemComponent* TargetCombatSystem =
			FindCombatSystemComponent(HitActor))
		{
			if (TargetCombatSystem->CanDefendAgainst(this))
			{
				FGameplayEventData HitEventData;
				HitEventData.Instigator = GetOwner();
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					HitActor,
					DungeonRealmsGameplayTags::Event_Guard_Hit,
					HitEventData
				);
				break;
			}
		}
		HitTargets.Add(HitActor);
	}

	if (HitTargets.Num() > 0)
	{
		FGameplayEventData HitEventData;
		HitEventData.Instigator = GetOwner();
		FGameplayAbilityTargetData_ActorArray* HitTargetData = new FGameplayAbilityTargetData_ActorArray();
		HitTargetData->SetActors(HitTargets);
		HitEventData.TargetData = FGameplayAbilityTargetDataHandle(HitTargetData);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwner(),
			DungeonRealmsGameplayTags::Event_Attack_Hit,
			HitEventData
		);
	}
}

void UDungeonRealmsCombatSystemComponent::EndAttackTrace()
{
	ActiveAttackTracer->EndTrace();
}

void UDungeonRealmsCombatSystemComponent::BeginGuard(float InDefenseDegrees)
{
	DefenseDegrees = InDefenseDegrees;
	bIsGuarding = true;
}

void UDungeonRealmsCombatSystemComponent::EndGuard()
{
	bIsGuarding = false;
}

bool UDungeonRealmsCombatSystemComponent::CanDefendAgainst(const UDungeonRealmsCombatSystemComponent* Attacker) const
{
	if (!bIsGuarding)
	{
		return false;
	}
	const AActor* OwningActor = GetOwner();
	const FVector ToAttacker = Attacker->GetOwner()->GetActorLocation() - OwningActor->GetActorLocation();
	const float DotResult = OwningActor->GetActorForwardVector().Dot(ToAttacker.GetSafeNormal2D());
	return UKismetMathLibrary::DegAcos(DotResult) <= DefenseDegrees;
}

void UDungeonRealmsCombatSystemComponent::ApplyDamageEffect(const FDamageSpec& DamageSpec)
{
	UAbilitySystemComponent* OwningAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());

	FGameplayEffectContextHandle EffectContext = OwningAbilitySystem->MakeEffectContext();
	EffectContext.AddSourceObject(DamageSpec.SourceObject.Get());
	EffectContext.AddInstigator(DamageSpec.Instigator.Get(), DamageSpec.DamageCauser.Get());

	FDungeonRealmsGameplayEffectContext* ExtraEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(EffectContext);
	ExtraEffectContext->SetDamageImpact(DamageSpec.DamageImpact);
	ExtraEffectContext->SetKnockbackPower(DamageSpec.KnockbackPower);
	ExtraEffectContext->SetKnockdown(DamageSpec.bShouldKnockdown);
	
	FGameplayEffectSpecHandle EffectSpec = OwningAbilitySystem->MakeOutgoingSpec(DamageSpec.DamageEffect, 1.0f, EffectContext);
	EffectSpec.Data->SetSetByCallerMagnitude(
		DungeonRealmsGameplayTags::SetByCaller_Damage_AttackDamage_Coefficient,
		DamageSpec.AttackDamageCoefficient
	);
	EffectSpec.Data->SetSetByCallerMagnitude(
		DungeonRealmsGameplayTags::SetByCaller_Damage_AbilityPower_Coefficient,
		DamageSpec.AbilityPowerCoefficient
	);
	
	OwningAbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data);
}
