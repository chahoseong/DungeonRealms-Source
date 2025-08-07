#include "AbilitySystem/Abilities/DungeonRealmsGameplayAbility.h"

bool UDungeonRealmsGameplayAbility::HasActivationPolicy(UGameplayAbility* Ability,
														EDungeonRealmsAbilityActivationPolicy ActivationPolicy)
{
	if (const UDungeonRealmsGameplayAbility* DungeonRealmsGameplayAbility = Cast<UDungeonRealmsGameplayAbility>(Ability))
	{
		return DungeonRealmsGameplayAbility->GetActivationPolicy() == ActivationPolicy;
	}
	return false;
}

UDungeonRealmsGameplayAbility::UDungeonRealmsGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDungeonRealmsGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
												 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	K2_InputPressed();
}

void UDungeonRealmsGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	K2_InputReleased();
}

EDungeonRealmsAbilityActivationPolicy UDungeonRealmsGameplayAbility::GetActivationPolicy() const
{
	return ActivationPolicy;
}

AController* UDungeonRealmsGameplayAbility::GetControllerFromActorInfo() const
{
	if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
	{
		if (ActorInfo->PlayerController.IsValid())
		{
			return ActorInfo->PlayerController.Get();
		}

		AActor* Actor = ActorInfo->OwnerActor.Get();
		while (IsValid(Actor))
		{
			AController* Controller = Cast<AController>(Actor);
			if (IsValid(Controller))
			{
				return Controller;
			}

			APawn* Pawn = Cast<APawn>(Actor);
			if (IsValid(Pawn))
			{
				return Pawn->GetController();
			}

			Actor = Actor->GetOwner();
		}
	}

	return nullptr;
}

bool UDungeonRealmsGameplayAbility::IsRunningOnDedicatedServer() const
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	return AvatarActor->GetNetMode() == NM_DedicatedServer;
}
