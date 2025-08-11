#include "CombatSystem/DungeonRealmsTargetLockComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DungeonRealmsGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

UDungeonRealmsTargetLockComponent::UDungeonRealmsTargetLockComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UDungeonRealmsTargetLockComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentLockedTarget);
}

void UDungeonRealmsTargetLockComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsOwnerDead())
	{
		return;
	}
	
	if (!CurrentLockedTarget || IsTargetDead())
	{
		SetTarget(nullptr);
		return;
	}

	const UAbilitySystemComponent* OwningAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	const bool bIsRolling =
		OwningAbilitySystem->HasMatchingGameplayTag(DungeonRealmsGameplayTags::State_Rolling);
	if (bIsRolling)
	{
		bShouldActorRotateTowardsTarget = true;
	}
	else
	{
		UpdateOwnerRotation(DeltaTime);
	}
}

bool UDungeonRealmsTargetLockComponent::IsTargetDead() const
{
	return IsActorDead(CurrentLockedTarget);
}

bool UDungeonRealmsTargetLockComponent::IsOwnerDead() const
{
	return IsActorDead(GetOwner());
}

bool UDungeonRealmsTargetLockComponent::IsActorDead(AActor* Actor) const
{
	const UAbilitySystemComponent* AbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	return IsValid(AbilitySystem)
		       ? AbilitySystem->HasMatchingGameplayTag(DungeonRealmsGameplayTags::State_Dead)
		       : false;
}

void UDungeonRealmsTargetLockComponent::SetTarget(AActor* InTarget)
{
	if (InTarget)
	{
		if (CurrentLockedTarget != InTarget)
		{
			if (CurrentLockedTarget)
			{
				if (const ACharacter* Character = Cast<ACharacter>(CurrentLockedTarget))
				{
					Character->GetCharacterMovement()->bOrientRotationToMovement = bCachedOrientRotationToMovement;
				}
			}
			if (const ACharacter* TargetCharacter = Cast<ACharacter>(InTarget))
			{
				bCachedOrientRotationToMovement = TargetCharacter->GetCharacterMovement()->bOrientRotationToMovement;
				TargetCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
			}
			OnTargetLocked.Broadcast();
		}
		bTargetLost = false;
	}
	else
	{
		if (CurrentLockedTarget)
		{
			if (const ACharacter* Character = Cast<ACharacter>(CurrentLockedTarget))
			{
				Character->GetCharacterMovement()->bOrientRotationToMovement = bCachedOrientRotationToMovement;
			}
		}
		if (!bTargetLost)
		{
			OnTargetLost.Broadcast();
		}
		bTargetLost = true;
	}
	CurrentLockedTarget = InTarget;
}

AActor* UDungeonRealmsTargetLockComponent::GetTarget() const
{
	return CurrentLockedTarget;
}

void UDungeonRealmsTargetLockComponent::UpdateOwnerRotation(float DeltaTime)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
		GetOwner()->GetActorLocation(),
		CurrentLockedTarget->GetActorLocation()
	);
	TargetRotation += FRotator(ViewPitchOffset, 0.0f, 0.0f);

	if (AController* Controller = GetOwningController())
	{
		const FRotator CurrentControlRotation = Controller->GetControlRotation();
		const FRotator ControlRotationStep = FMath::RInterpTo(CurrentControlRotation, TargetRotation, DeltaTime,
		                                                      RotationSpeed);
		Controller->SetControlRotation(FRotator(ControlRotationStep.Pitch, ControlRotationStep.Yaw, 0.0f));

		if (bShouldActorRotateTowardsTarget)
		{
			if (IsFacingTarget())
			{
				bShouldActorRotateTowardsTarget = false;
			}
			else
			{
				const FRotator ActorRotationStep = FMath::RInterpTo(
					GetOwner()->GetActorRotation(),
					TargetRotation,
					DeltaTime,
					RotationSpeed
				);
				GetOwner()->SetActorRotation(FRotator(0.0f, ActorRotationStep.Yaw, 0.0f));
			}
		}
		else
		{
			GetOwner()->SetActorRotation(FRotator(0.0f, ControlRotationStep.Yaw, 0.0f));
		}
	}
}

bool UDungeonRealmsTargetLockComponent::IsFacingTarget(float Tolerance) const
{
	const FVector ToTarget = (CurrentLockedTarget->GetActorLocation() - GetOwner()->GetActorLocation()).
	GetSafeNormal();
	const float DotResult = GetOwner()->GetActorForwardVector().Dot(ToTarget);
	return DotResult >= Tolerance;
}

AController* UDungeonRealmsTargetLockComponent::GetOwningController() const
{
	if (const APawn* OwningPawn = GetOwner<APawn>())
	{
		return OwningPawn->GetController();
	}
	return nullptr;
}

void UDungeonRealmsTargetLockComponent::OnRep_CurrentLockedTarget(AActor* LastLockedTarget)
{
	if (CurrentLockedTarget != LastLockedTarget)
	{
		SetTarget(CurrentLockedTarget);
	}
}
