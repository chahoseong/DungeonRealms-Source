#include "AbilitySystem/DungeonRealmsAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/DungeonRealmsGameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "DungeonRealmsGameplayTags.h"

UDungeonRealmsAbilitySystemComponent* UDungeonRealmsAbilitySystemComponent::FindAbilitySystemComponent(const AActor* Actor)
{
	if (IsValid(Actor))
	{
		UAbilitySystemComponent* AbilitySystemComponent = nullptr;
		
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Actor))
		{
			AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
		}

		if (!IsValid(AbilitySystemComponent))
		{
			AbilitySystemComponent = Actor->FindComponentByClass<UAbilitySystemComponent>();
		}
		
		return Cast<UDungeonRealmsAbilitySystemComponent>(AbilitySystemComponent);
	}
	
	return nullptr;
}

UDungeonRealmsAbilitySystemComponent::UDungeonRealmsAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ClearAbilityInput();
}

void UDungeonRealmsAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (IsValid(AbilitySpec.Ability) && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
		}
	}
}

void UDungeonRealmsAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (IsValid(AbilitySpec.Ability) && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
		}
	}
}

void UDungeonRealmsAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (IsValid(AbilitySpec.Ability) && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
			InputHeldSpecHandles.Remove(AbilitySpec.Handle);
		}
	}
}

void UDungeonRealmsAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(DungeonRealmsGameplayTags::Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	// Process all abilities that activate when the input is held.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (!IsValid(AbilitySpec->Ability) || AbilitySpec->IsActive())
			{
				continue;
			}
			
			if (UDungeonRealmsGameplayAbility::HasActivationPolicy(AbilitySpec->Ability, EDungeonRealmsAbilityActivationPolicy::WhileInputActive))
			{
				AbilitiesToActivate.Add(AbilitySpec->Handle);
			}
		}
	}

	// Process all abilities that had their input pressed this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (!IsValid(AbilitySpec->Ability))
			{
				continue;
			}

			AbilitySpec->InputPressed = true;

			if (AbilitySpec->IsActive())
			{
				AbilitySpecInputPressed(*AbilitySpec);
			}
			else if (UDungeonRealmsGameplayAbility::HasActivationPolicy(AbilitySpec->Ability, EDungeonRealmsAbilityActivationPolicy::OnInputTriggered))
			{
				AbilitiesToActivate.Add(AbilitySpec->Handle);
			}
		}
	}

	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	// Process all abilities that had their input released this frame.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle))
		{
			if (!IsValid(AbilitySpec->Ability))
			{
				continue;
			}

			AbilitySpec->InputPressed = false;

			if (AbilitySpec->IsActive())
			{
				AbilitySpecInputReleased(*AbilitySpec);
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UDungeonRealmsAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
