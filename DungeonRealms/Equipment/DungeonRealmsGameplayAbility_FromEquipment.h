#pragma once

#include "AbilitySystem/Abilities/DungeonRealmsGameplayAbility.h"
#include "DungeonRealmsGameplayAbility_FromEquipment.generated.h"

class UDungeonRealmsEquipmentInstance;

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsGameplayAbility_FromEquipment : public UDungeonRealmsGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UDungeonRealmsEquipmentInstance* GetAssociatedEquipment() const;
};
