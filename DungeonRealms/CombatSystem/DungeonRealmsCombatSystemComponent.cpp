#include "CombatSystem/DungeonRealmsCombatSystemComponent.h"
#include "CombatSystem/DungeonRealmsCombatStatics.h"
#include "CombatSystem/DungeonRealmsAttackTracer.h"
#include "CombatSystem/DungeonRealmsCombatSystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DungeonRealmsGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Character/DungeonRealmsCharacter.h"
#include "DungeonRealmsLogChannels.h"

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

	TArray<FHitResult> Hits = ActiveAttackTracer->PerformTrace();

	// TODO: 적대적인 액터만 골라내야 함.
	
	ApplyHitEvents(Hits);
}

void UDungeonRealmsCombatSystemComponent::ApplyHitEvents(const TArray<FHitResult>& Hits) const
{
	TArray<TWeakObjectPtr<AActor>> HitTargets;
	
	FGameplayEventData HitEventData;
	HitEventData.Instigator = GetOwner();

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		const UDungeonRealmsCombatSystemComponent* TargetCombatSystem =
			FindCombatSystemComponent(HitActor);
		if (IsValid(TargetCombatSystem); TargetCombatSystem->IsGuarding())
		{
			FGameplayAbilityTargetData_SingleTargetHit* HitTargetData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
			HitEventData.TargetData = FGameplayAbilityTargetDataHandle(HitTargetData);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				HitActor,
				DungeonRealmsGameplayTags::Event_Attack_Hit,
				HitEventData
			);
			break;
		}
		HitTargets.Add(HitActor);
	}

	if (HitTargets.Num() > 0)
	{
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

bool UDungeonRealmsCombatSystemComponent::IsGuarding() const
{
	UAbilitySystemComponent* AbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	return IsValid(AbilitySystem) ? AbilitySystem->HasMatchingGameplayTag(DungeonRealmsGameplayTags::State_Guarding) : false;
}

void UDungeonRealmsCombatSystemComponent::EndAttackTrace()
{
	ActiveAttackTracer->EndTrace();
}

bool UDungeonRealmsCombatSystemComponent::CanDefendAgainst(const AActor* Attacker, float DefenseDegrees) const
{
	const AActor* OwningActor = GetOwner();
	FVector ToAttacker = Attacker->GetActorLocation() - OwningActor->GetActorLocation();
	ToAttacker.Z = 0.0f;
	float DotResult = OwningActor->GetActorForwardVector().Dot(ToAttacker.GetSafeNormal());
	return FMath::Acos(DotResult) <= DefenseDegrees;
}
