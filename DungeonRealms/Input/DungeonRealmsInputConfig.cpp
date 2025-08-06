#include "Input/DungeonRealmsInputConfig.h"
#include "DungeonRealmsLogChannels.h"

UDungeonRealmsInputConfig::UDungeonRealmsInputConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UInputAction* UDungeonRealmsInputConfig::FindNativeInputActionWithTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FDungeonRealmsInputAction& Action : NativeInputActions)
	{
		if (IsValid(Action.InputAction) && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogDungeonRealms, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UDungeonRealmsInputConfig::FindAbilityInputActionWithTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FDungeonRealmsInputAction& Action : AbilityInputActions)
	{
		if (IsValid(Action.InputAction) && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogDungeonRealms, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
