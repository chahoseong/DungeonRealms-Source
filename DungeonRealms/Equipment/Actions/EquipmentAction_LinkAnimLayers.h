#pragma once

#include "DungeonRealmsEquipmentAction.h"
#include "EquipmentAction_LinkAnimLayers.generated.h"

UCLASS(DisplayName="Link AnimLayer")
class DUNGEONREALMS_API UEquipmentAction_LinkAnimLayers : public UDungeonRealmsEquipmentAction
{
	GENERATED_BODY()

public:
	virtual void OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;
	virtual void OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimInstance> AnimLayerToLink;
};
