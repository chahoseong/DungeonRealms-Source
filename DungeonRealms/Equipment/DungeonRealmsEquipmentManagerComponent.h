#pragma once

#include "Components/ActorComponent.h"
#include "Equipment/DungeonRealmsEquipmentList.h"
#include "DungeonRealmsEquipmentManagerComponent.generated.h"

struct FGameplayTag;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEONREALMS_API UDungeonRealmsEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDungeonRealmsEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UDungeonRealmsEquipmentInstance* Equip(TSubclassOf<UDungeonRealmsEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Unequip(UDungeonRealmsEquipmentInstance* EquipmentInstance);

	UFUNCTION(BlueprintCallable)
	UDungeonRealmsEquipmentInstance* GetEquipmentBySlot(const FGameplayTag& SlotTag);

	//~Begin UObject interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End UObject interface

	//~Begin UActorComponent interface
	virtual void ReadyForReplication() override;
	//~End UActorComponent interface

protected:
	virtual void OnRegister() override;

private:
	void OnEquipmentReplicatedAdd(UDungeonRealmsEquipmentInstance* EquipmentInstance);
	void OnEquipmentReplicatedRemove(UDungeonRealmsEquipmentInstance* EquipmentInstance);
	
protected:
	UPROPERTY(Replicated)
	FDungeonRealmsEquipmentList EquipmentList;
};
