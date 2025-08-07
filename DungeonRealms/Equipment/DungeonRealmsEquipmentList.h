#pragma once

#include "Net/Serialization/FastArraySerializer.h"
#include "DungeonRealmsEquipmentList.generated.h"

class UDungeonRealmsEquipmentDefinition;
class UDungeonRealmsEquipmentInstance;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentReplicated, UDungeonRealmsEquipmentInstance*)

USTRUCT(BlueprintType)
struct FDungeonRealmsAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UDungeonRealmsEquipmentInstance> EquipmentInstance;
};

USTRUCT(BlueprintType)
struct FDungeonRealmsEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDungeonRealmsAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;

	FDungeonRealmsEquipmentList(UActorComponent* InOwnerComponent = nullptr);

	UDungeonRealmsEquipmentInstance* AddEntry(TSubclassOf<UDungeonRealmsEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UDungeonRealmsEquipmentInstance* EquipmentInstance);

	FOnEquipmentReplicated OnEquipmentReplicatedAdd;
	FOnEquipmentReplicated OnEquipmentReplicatedRemove;

	//~Begin FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams);
	//~End FFastArraySerializer contract
};

template <>
struct TStructOpsTypeTraits<FDungeonRealmsEquipmentList> : TStructOpsTypeTraitsBase2<FDungeonRealmsEquipmentList>
{
	enum { WithNetDeltaSerializer = true };	
};
