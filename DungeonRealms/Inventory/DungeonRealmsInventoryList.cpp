#include "Inventory/DungeonRealmsInventoryList.h"

UDungeonRealmsInventoryItemInstance* FDungeonRealmsInventoryList::AddEntry(
	TSubclassOf<UDungeonRealmsInventoryItemDefinition> ItemDefinition, int32 StackCount)
{
	return nullptr;
}

void FDungeonRealmsInventoryList::RemoveEntry(UDungeonRealmsInventoryItemInstance* ItemInstance)
{
}

TArray<UDungeonRealmsInventoryItemInstance*> FDungeonRealmsInventoryList::GetItems() const
{
	return TArray<UDungeonRealmsInventoryItemInstance*>();
}

void FDungeonRealmsInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FDungeonRealmsInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

void FDungeonRealmsInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

bool FDungeonRealmsInventoryList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
{
	return FastArrayDeltaSerialize<FDungeonRealmsInventoryEntry, FDungeonRealmsInventoryList>(Entries, DeltaParams, *this);
}
