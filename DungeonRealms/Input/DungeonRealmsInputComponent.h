#pragma once

#include "EnhancedInputComponent.h"
#include "Input/DungeonRealmsInputConfig.h"
#include "DungeonRealmsInputComponent.generated.h"

struct FGameplayTag;

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename FuncType>
	void BindNativeAction(
		const UDungeonRealmsInputConfig* InputConfig,
		const FGameplayTag& InputTag,
		ETriggerEvent TriggerEvent,
		UserClass* Object,
		FuncType Func,
		bool bLogIfNotFound
	);

	template <class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
	void BindAbilityActions(
		const UDungeonRealmsInputConfig* InputConfig,
		UserClass* Object,
		PressedFuncType PressedFunc,
		HeldFuncType HeldFunc,
		ReleasedFuncType ReleasedFunc
	);
};

template <class UserClass, typename FuncType>
void UDungeonRealmsInputComponent::BindNativeAction(const UDungeonRealmsInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	if (const UInputAction* InputAction = InputConfig->FindNativeInputActionWithTag(InputTag, bLogIfNotFound))
	{
		BindAction(InputAction, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
void UDungeonRealmsInputComponent::BindAbilityActions(const UDungeonRealmsInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const FDungeonRealmsInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
		}
	}
}
