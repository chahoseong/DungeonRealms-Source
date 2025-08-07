#pragma once

#include "DungeonRealmsEquipmentFeature.h"
#include "EquipmentFeature_LinkAnimLayers.generated.h"

UCLASS(DisplayName="Link AnimLayer")
class DUNGEONREALMS_API UEquipmentFeature_LinkAnimLayers : public UDungeonRealmsEquipmentFeature
{
	GENERATED_BODY()

public:
	virtual void OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;
	virtual void OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimInstance> AnimLayerToLink;
};
