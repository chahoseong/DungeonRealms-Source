#include "CombatSystem/DungeonRealmsCombatSystemComponent.h"
#include "CombatSystem/DungeonRealmsCombatStatics.h"
#include "CombatSystem/FDungeonRealmsAttackTracer.h"
#include "CombatSystem/DungeonRealmsCombatSystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
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
		
		if (UDungeonRealmsCombatSystemComponent* CombatSystemComponent = Actor->FindComponentByClass<UDungeonRealmsCombatSystemComponent>())
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
}

void UDungeonRealmsCombatSystemComponent::BeginAttackTrace(FName SocketName)
{
	const ADungeonRealmsCharacter* OwningCharacter = GetOwner<ADungeonRealmsCharacter>();
	AttackTraceSourceActor = OwningCharacter->GetAttachedActorFromSocket(SocketName);
	if (AttackTraceSourceActor.IsValid())
	{
		if (UPrimitiveComponent* HitboxComponent =
			AttackTraceSourceActor->FindComponentByTag<UPrimitiveComponent>(FName("Hitbox")))
		{
			AttackTracer.BeginTrace(
				HitboxComponent,
				AttackTraceObjectTypes,
				{ GetOwner(), AttackTraceSourceActor.Get() }
			);
		}
		else
		{
			UE_LOG(LogDungeonRealms, Error, TEXT("%s actor doesn't have component with hitbox tag."), *AttackTraceSourceActor->GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogDungeonRealms, Error, TEXT("Attached actor not found to %s socket."), *SocketName.ToString());
	}
}

void UDungeonRealmsCombatSystemComponent::PerformAttackTrace()
{
	if (!AttackTraceSourceActor.IsValid())
	{
		return;
	}
	if (UDungeonRealmsCombatStatics::HasObstacleBetween(GetOwner(), AttackTraceSourceActor.Get()))
	{
		return;
	}
	const TArray<FHitResult> Hits =
		AttackTracer.PerformTrace(AttackTraceSubsteps, bDrawDebugAttackTrace);
	const TArray<FHitResult> HostileTargets = FilterToHostileTargets(Hits);
	ProcessHitEvents(HostileTargets);
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

void UDungeonRealmsCombatSystemComponent::ProcessHitEvents(const TArray<FHitResult>& Hits) const
{
	TArray<FHitEventData> HitEvents;
	
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		
		FHitEventData HitEvent;
		HitEvent.Instigator = GetOwner();
		HitEvent.TargetActor = HitActor;
		
		if (const UDungeonRealmsCombatSystemComponent* TargetCombatSystem =
			FindCombatSystemComponent(HitActor))
		{
			if (TargetCombatSystem->IsGuardStance())
			{
				HitEvent = TargetCombatSystem->ResolveHitEvent(GetOwner(), HitEvent);
			}
		}
		
		HitEvents.Add(HitEvent);

		if (HitEvent.bParried)
		{
			break;
		}
	}

	if (HitEvents.Num() > 0)
	{
		OnAttackHits.Broadcast(HitEvents);
	}
}

void UDungeonRealmsCombatSystemComponent::EndAttackTrace()
{
	AttackTracer.EndTrace();
}

void UDungeonRealmsCombatSystemComponent::SetDefensibleAngle(float Degrees)
{
	DefensibleDegrees = Degrees;
}

void UDungeonRealmsCombatSystemComponent::SetParryable(bool bEnabled)
{
	bParryable = bEnabled;
}

bool UDungeonRealmsCombatSystemComponent::IsParryable() const
{
	return bParryable;
}

bool UDungeonRealmsCombatSystemComponent::IsGuardStance() const
{
	UAbilitySystemComponent* OwningAbilitySystem =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	return IsValid(OwningAbilitySystem)
		       ? OwningAbilitySystem->HasMatchingGameplayTag(DungeonRealmsGameplayTags::State_Guarding)
		       : false;
}

FHitEventData UDungeonRealmsCombatSystemComponent::ResolveHitEvent(
	const AActor* Instigator, const FHitEventData& Payload) const
{
	const AActor* OwningActor = GetOwner();
	const FVector ToAttacker = Instigator->GetActorLocation() - OwningActor->GetActorLocation();
	const float DotResult = OwningActor->GetActorForwardVector().Dot(ToAttacker.GetSafeNormal2D());

	FHitEventData HitEvent = Payload;
	if (UKismetMathLibrary::DegAcos(DotResult) <= DefensibleDegrees)
	{
		if (IsParryable())
		{
			HitEvent.bParried = true;
		}
		else
		{
			HitEvent.bBlocked = true;
		}
		OnGuardHits.Broadcast({HitEvent});
	}
	return HitEvent;
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
	ExtraEffectContext->SetAttackBlocked(DamageSpec.bAttackBlocked);
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
