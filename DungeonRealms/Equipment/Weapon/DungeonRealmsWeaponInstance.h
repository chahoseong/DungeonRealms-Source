#pragma once

#include "CoreMinimal.h"
#include "Equipment/DungeonRealmsEquipmentInstance.h"
#include "DungeonRealmsWeaponInstance.generated.h"

class UInputMappingContext;
class UDungeonRealmsWeaponDefinition;

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsWeaponInstance : public UDungeonRealmsEquipmentInstance
{
	GENERATED_BODY()

public:
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;

private:
	void AddInputMappingContext(const UInputMappingContext* InputMappingContext, int32 Priority);
	void RemoveInputMappingContext(const UInputMappingContext* InputMappingContext);

	void LinkAnimClassLayers(TSubclassOf<UAnimInstance> AnimClassLayers);
	void UnlinkAnimClassLayers(TSubclassOf<UAnimInstance> AnimClassLayers);
	
	void GiveAbilitySetsToOwner(const TArray<UDungeonRealmsAbilitySet*>& AbilitySets);
	void TakeAbilitySetsFromOwner();

private:
	FDungeonRealmsAbilitySet_GrantedHandles GrantedHandles;
	bool bInputMappingContextAdded = false;
	bool bAnimClassLayersLinked = false;
};
