#include "AbilitySystem/Abilities/Tasks/AbilityTask_GetLastMovementInput.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UAbilityTask_GetLastMovementInput* UAbilityTask_GetLastMovementInput::GetLastMovementInput(
	UGameplayAbility* OwningAbility)
{
	UAbilityTask_GetLastMovementInput* Task = NewAbilityTask<UAbilityTask_GetLastMovementInput>(OwningAbility);
	return Task;
}

UAbilityTask_GetLastMovementInput::UAbilityTask_GetLastMovementInput(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UAbilityTask_GetLastMovementInput::Activate()
{
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendLastMovementInput();
	}
	else
	{
		if (AbilitySystemComponent.IsValid())
		{
			FSimpleMulticastDelegate& Delegate = AbilitySystemComponent->AbilityReplicatedEventDelegate(
				ReplicatedEventType,
				GetAbilitySpecHandle(),
				GetActivationPredictionKey()
			);
			ReplicatedEventDelegate = Delegate.AddUObject(this, &ThisClass::OnLastMovementInputReplicated);

			const bool bIsDataSet = AbilitySystemComponent->CallReplicatedEventDelegateIfSet(
				ReplicatedEventType,
				GetAbilitySpecHandle(),
				GetActivationPredictionKey()
			);
			if (!bIsDataSet)
			{
				SetWaitingOnRemotePlayerData();
			}
		}
	}
}

void UAbilityTask_GetLastMovementInput::OnDestroy(bool bInOwnerFinished)
{
	if (ReplicatedEventDelegate.IsValid())
	{
		FSimpleMulticastDelegate& Delegate = AbilitySystemComponent->AbilityReplicatedEventDelegate(
			ReplicatedEventType,
			GetAbilitySpecHandle(),
			GetActivationPredictionKey()
		);
		Delegate.Remove(ReplicatedEventDelegate);
	}
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_GetLastMovementInput::SendLastMovementInput() const
{
	if (AbilitySystemComponent.IsValid())
	{
		if (const ACharacter* OwningCharacter = Cast<ACharacter>(GetAvatarActor()))
		{
			const FVector LastMovementInput = OwningCharacter->GetLastMovementInputVector();
			AbilitySystemComponent->ServerSetReplicatedEventWithPayload(
				ReplicatedEventType,
				GetAbilitySpecHandle(),
				GetActivationPredictionKey(),
				AbilitySystemComponent->ScopedPredictionKey,
				LastMovementInput
			);

			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnLastMovementInput.Broadcast(LastMovementInput);
			}
		}
	}
}

void UAbilityTask_GetLastMovementInput::OnLastMovementInputReplicated()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->ConsumeGenericReplicatedEvent(
			ReplicatedEventType,
			GetAbilitySpecHandle(),
			GetActivationPredictionKey()
		);

		const FAbilityReplicatedData Data = AbilitySystemComponent->GetReplicatedDataOfGenericReplicatedEvent(
			ReplicatedEventType,
			GetAbilitySpecHandle(),
			GetActivationPredictionKey()
		);
		
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnLastMovementInput.Broadcast(Data.VectorPayload);
		}
	}
}
