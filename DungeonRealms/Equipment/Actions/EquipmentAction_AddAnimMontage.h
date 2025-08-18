#pragma once

#include "DungeonRealmsEquipmentAction.h"
#include "GameplayTagContainer.h"
#include "EquipmentAction_AddAnimMontage.generated.h"

UCLASS(DisplayName="Add AnimMontage")
class DUNGEONREALMS_API UEquipmentAction_AddAnimMontage : public UDungeonRealmsEquipmentAction
{
	GENERATED_BODY()

public:
	virtual void OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;
	virtual void OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> AnimMontages;
};
