#include "Equipment/Actions/EquipmentAction_AddInputMapping.h"
#include "Equipment/DungeonRealmsEquipmentInstance.h"
#include "EnhancedInputSubsystems.h"

void UEquipmentAction_AddInputMapping::OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	if (!EquipmentInstance->IsLocallyControlled())
	{
		return;
	}
	
	APawn* OwningPawn = EquipmentInstance->GetPawn();
	if (!IsValid(OwningPawn))
	{
		return;
	}

	APlayerController* PlayerController = OwningPawn->GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		for (const FInputMappingContextToApply& ContextToApply : InputMappingContextsToApply)
		{
			Subsystem->AddMappingContext(ContextToApply.InputMappingContext, ContextToApply.Priority);
		}
	}
}

void UEquipmentAction_AddInputMapping::OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	if (!EquipmentInstance->IsLocallyControlled())
	{
		return;
	}
	
	APawn* OwningPawn = EquipmentInstance->GetPawn();
	if (!IsValid(OwningPawn))
	{
		return;
	}

	APlayerController* PlayerController = OwningPawn->GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		for (const FInputMappingContextToApply& ContextToApply : InputMappingContextsToApply)
		{
			Subsystem->RemoveMappingContext(ContextToApply.InputMappingContext);
		}
	}
}
