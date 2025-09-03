#pragma once

#include "Net/Serialization/FastArraySerializer.h"
#include "DungeonRealmsInventoryList.generated.h"

class UDungeonRealmsInventoryItemDefinition;
class UDungeonRealmsInventoryItemInstance;

USTRUCT(BlueprintType)
struct FDungeonRealmsInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDungeonRealmsInventoryItemInstance> ItemInstance;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FDungeonRealmsInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDungeonRealmsInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;

	explicit FDungeonRealmsInventoryList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{
	}

	UDungeonRealmsInventoryItemInstance* AddEntry(TSubclassOf<UDungeonRealmsInventoryItemDefinition> ItemDefinition, int32 StackCount);
	void RemoveEntry(UDungeonRealmsInventoryItemInstance* ItemInstance);
	
	TArray<UDungeonRealmsInventoryItemInstance*> GetItems() const;


	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams);
};

template<>
struct TStructOpsTypeTraits<FDungeonRealmsInventoryList> : TStructOpsTypeTraitsBase2<FDungeonRealmsInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};
