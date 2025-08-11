#include "AbilitySystem/Abilities/DungeonRealmsGameplayAbility_LockOn.h"
#include "CombatSystem/DungeonRealmsTargetLockComponent.h"
#include "DungeonRealmsGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GenericTeamAgentInterface.h"
#include "CombatSystem/DungeonRealmsCombatStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UDungeonRealmsGameplayAbility_LockOn::UDungeonRealmsGameplayAbility_LockOn(
	const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDungeonRealmsGameplayAbility_LockOn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsLocallyControlled())
	{
		AddInputMapping();
	}

	TryLockOnTarget();
}

void UDungeonRealmsGameplayAbility_LockOn::AddInputMapping() const
{
	if (const APlayerController* PlayerController = GetControllerFromActorInfo<APlayerController>())
	{
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMapping, InputPriority);
		}
	}
}

void UDungeonRealmsGameplayAbility_LockOn::TryLockOnTarget()
{
	AvailableActorsToLock = FindAvailableActorsToLock();
	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLock();
		return;
	}

	AActor* TargetActor = GetNearestTargetFromActors(GetAvatarActorFromActorInfo()->GetActorLocation(), AvailableActorsToLock);
	if (!TargetActor)
	{
		CancelTargetLock();
		return;
	}

	if (!TargetLockComponent.IsValid())
	{
		TargetLockComponent = GetAvatarActorFromActorInfo()->FindComponentByClass<UDungeonRealmsTargetLockComponent>();
	}
	
	if (TargetLockComponent.IsValid())
	{
		TargetLockComponent->OnTargetLost.AddDynamic(this, &ThisClass::OnTargetLost);
		TargetLockComponent->SetTarget(TargetActor);
	}
	else
	{
		CancelTargetLock();
	}
}

void UDungeonRealmsGameplayAbility_LockOn::EndAbility(const FGameplayAbilitySpecHandle Handle,
														  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
														  bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsLocallyControlled())
	{
		RemoveInputMapping();
	}
	if (TargetLockComponent.IsValid())
	{
		TargetLockComponent->OnTargetLost.RemoveDynamic(this, &ThisClass::OnTargetLost);
		TargetLockComponent->SetTarget(nullptr);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDungeonRealmsGameplayAbility_LockOn::RemoveInputMapping() const
{
	if (const APlayerController* PlayerController = GetControllerFromActorInfo<APlayerController>())
	{
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (Subsystem)
		{
			Subsystem->RemoveMappingContext(InputMapping);
		}
	}
}

void UDungeonRealmsGameplayAbility_LockOn::SwitchTarget(const FGameplayTag& DirectionTag)
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		ServerSwitchTarget_Implementation(DirectionTag);
	}
	else
	{
		ServerSwitchTarget(DirectionTag);
		HandleTargetSwitching(DirectionTag);
	}
}

void UDungeonRealmsGameplayAbility_LockOn::ServerSwitchTarget_Implementation(const FGameplayTag& DirectionTag)
{
	HandleTargetSwitching(DirectionTag);
}

void UDungeonRealmsGameplayAbility_LockOn::HandleTargetSwitching(const FGameplayTag& DirectionTag)
{
	if (!TargetLockComponent.IsValid())
	{
		CancelTargetLock();
		return;
	}
	
	AvailableActorsToLock = FindAvailableActorsToLock();

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	DivideAvailableActorsOnDirection(ActorsOnLeft, ActorsOnRight);

	AActor* NewTarget = nullptr;
	AActor* CurrentTarget = TargetLockComponent->GetTarget();
	if (DirectionTag == DungeonRealmsGameplayTags::Event_SwitchTarget_Left)
	{
		NewTarget = GetNearestTargetFromActors(CurrentTarget->GetActorLocation(), ActorsOnLeft);
	}
	else
	{
		NewTarget = GetNearestTargetFromActors(CurrentTarget->GetActorLocation(), ActorsOnRight);
	}
	if (NewTarget)
	{
		TargetLockComponent->SetTarget(NewTarget);
	}
}

void UDungeonRealmsGameplayAbility_LockOn::CancelTargetLock()
{
	CancelAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		true
	);
}

TArray<AActor*> UDungeonRealmsGameplayAbility_LockOn::FindAvailableActorsToLock() const
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	FRotator ControlRotation(0.0f, 0.0f, 0.0f);
	FVector ControlForward = FVector::ForwardVector;
	const AController* Controller = GetControllerFromActorInfo();
	
	if (IsValid(Controller))
	{
		ControlRotation = Controller->GetControlRotation();
		ControlForward = FRotationMatrix(ControlRotation).TransformVector(FVector::ForwardVector);
		ControlForward.Z = 0.0f;
	}
	
	TArray<FHitResult> Hits;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		AvatarActor,
		AvatarActor->GetActorLocation() + ControlForward * TraceBoxExtend.X,
		AvatarActor->GetActorLocation() + ControlForward * TraceMaxDistance,
		TraceBoxExtend,
		FRotator(0.0f, ControlRotation.Yaw, 0.0f),
		TraceQueryChannel,
		false,
		TArray<AActor*>(),
		bShowTraceDebug ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		Hits,
		true
	);

	TArray<AActor*> HitActors;
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (UDungeonRealmsCombatStatics::HasHostileAttitude(AvatarActor, HitActor))
		{
			HitActors.AddUnique(HitActor);
		}
	}
	
	return HitActors;
}

AActor* UDungeonRealmsGameplayAbility_LockOn::GetNearestTargetFromActors(const FVector& Origin, const TArray<AActor*>& ActorsToCheck) const
{
	float ClosestDistance = 0.0f;
	return UGameplayStatics::FindNearestActor(Origin, ActorsToCheck, ClosestDistance);
}

void UDungeonRealmsGameplayAbility_LockOn::DivideAvailableActorsOnDirection(TArray<AActor*>& ActorsOnLeft,
	TArray<AActor*>& ActorsOnRight) const
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const AActor* TargetActor = TargetLockComponent->GetTarget();
	const FVector BaseDirection = (TargetActor->GetActorLocation() - AvatarActor->GetActorLocation()).GetSafeNormal();

	for (AActor* Actor : AvailableActorsToLock)
	{
		if (Actor != TargetActor)
		{
			FVector ToActor = (Actor->GetActorLocation() - AvatarActor->GetActorLocation()).GetSafeNormal();
			FVector CrossResult = BaseDirection.Cross(ToActor);
			if (CrossResult.Z > 0.0f)
			{
				ActorsOnRight.Add(Actor);
			}
			else
			{
				ActorsOnLeft.Add(Actor);
			}
		}
	}
}

void UDungeonRealmsGameplayAbility_LockOn::OnTargetLost()
{
	CancelTargetLock();
}
