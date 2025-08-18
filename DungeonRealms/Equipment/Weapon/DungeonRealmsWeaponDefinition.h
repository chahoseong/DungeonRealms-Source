#pragma once

#include "Equipment/DungeonRealmsEquipmentDefinition.h"
#include "DungeonRealmsWeaponDefinition.generated.h"

class UDungeonRealmsAbilitySet;
class UInputMappingContext;

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsWeaponDefinition : public UDungeonRealmsEquipmentDefinition
{
	GENERATED_BODY()

public:
	UDungeonRealmsWeaponDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	int32 InputPriority = 1;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UAnimInstance> AnimClassLayers;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TArray<TObjectPtr<UDungeonRealmsAbilitySet>> AbilitySets;
};
