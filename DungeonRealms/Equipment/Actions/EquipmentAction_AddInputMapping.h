#pragma once

#include "DungeonRealmsEquipmentAction.h"
#include "EquipmentAction_AddInputMapping.generated.h"

class UInputMappingContext;

USTRUCT()
struct FInputMappingContextToApply
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	int32 Priority = 1;
};

UCLASS(DisplayName="Add InputMapping")
class DUNGEONREALMS_API UEquipmentAction_AddInputMapping : public UDungeonRealmsEquipmentAction
{
	GENERATED_BODY()

public:
	virtual void OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;
	virtual void OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FInputMappingContextToApply> InputMappingContextsToApply;
};
