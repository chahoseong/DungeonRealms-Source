#include "AbilitySystem/Abilities/DungeonRealmsGameplayAbility_Jump.h"
#include "Characters/DungeonRealmsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UDungeonRealmsGameplayAbility_Jump::UDungeonRealmsGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UDungeonRealmsGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ADungeonRealmsCharacter* DungeonRealmsCharacter =
		Cast<ADungeonRealmsCharacter>(ActorInfo->AvatarActor.Get());
	if (!DungeonRealmsCharacter || !DungeonRealmsCharacter->CanJump())
	{
		return false;
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UDungeonRealmsGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ADungeonRealmsCharacter* DungeonRealmsCharacter = GetDungeonRealmsCharacterFromActorInfo())
	{
		UCharacterMovementComponent* CharacterMovement = DungeonRealmsCharacter->GetCharacterMovement();
		CachedJumpSpeed = CharacterMovement->JumpZVelocity;
		CharacterMovement->JumpZVelocity = JumpSpeed;
	}
}

void UDungeonRealmsGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();

	if (ADungeonRealmsCharacter* DungeonRealmsCharacter = GetDungeonRealmsCharacterFromActorInfo())
	{
		UCharacterMovementComponent* CharacterMovement = DungeonRealmsCharacter->GetCharacterMovement();
		CharacterMovement->JumpZVelocity = CachedJumpSpeed;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDungeonRealmsGameplayAbility_Jump::CharacterJumpStart()
{
	if (ADungeonRealmsCharacter* DungeonRealmsCharacter = GetDungeonRealmsCharacterFromActorInfo())
	{
		if (!DungeonRealmsCharacter->bPressedJump)
		{
			DungeonRealmsCharacter->Jump();
		}
	}
}

void UDungeonRealmsGameplayAbility_Jump::CharacterJumpStop()
{
	if (ADungeonRealmsCharacter* DungeonRealmsCharacter = GetDungeonRealmsCharacterFromActorInfo())
	{
		if (DungeonRealmsCharacter->bPressedJump)
		{
			DungeonRealmsCharacter->StopJumping();
		}
	}
}
