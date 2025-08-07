#include "Equipment/DungeonRealmsEquipmentList.h"
#include "Equipment/DungeonRealmsEquipmentDefinition.h"
#include "Equipment/DungeonRealmsEquipmentInstance.h"

FDungeonRealmsEquipmentList::FDungeonRealmsEquipmentList(UActorComponent* InOwnerComponent)
	: OwnerComponent(InOwnerComponent)
{
}

UDungeonRealmsEquipmentInstance* FDungeonRealmsEquipmentList::AddEntry(
	TSubclassOf<UDungeonRealmsEquipmentDefinition> EquipmentDefinition)
{
	check(EquipmentDefinition);
	check(IsValid(OwnerComponent) && IsValid(OwnerComponent->GetOwner()));
	check(OwnerComponent->GetOwner()->HasAuthority());

	const UDungeonRealmsEquipmentDefinition* EquipmentDefault = GetDefault<UDungeonRealmsEquipmentDefinition>(EquipmentDefinition);
	TSubclassOf<UDungeonRealmsEquipmentInstance> EquipmentInstanceClass = EquipmentDefault->EquipmentInstanceClass;
	if (!EquipmentInstanceClass)
	{
		EquipmentInstanceClass = UDungeonRealmsEquipmentInstance::StaticClass();
	}

	FDungeonRealmsAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentInstance = NewObject<UDungeonRealmsEquipmentInstance>(OwnerComponent->GetOwner(), EquipmentInstanceClass);

	MarkItemDirty(NewEntry);

	return NewEntry.EquipmentInstance;
}

void FDungeonRealmsEquipmentList::RemoveEntry(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	for (auto It = Entries.CreateIterator(); It; ++It)
	{
		FDungeonRealmsAppliedEquipmentEntry& Entry = *It;
		if (Entry.EquipmentInstance == EquipmentInstance)
		{
			It.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

void FDungeonRealmsEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FDungeonRealmsAppliedEquipmentEntry& Entry = Entries[Index];
		if (IsValid(Entry.EquipmentInstance))
		{
			OnEquipmentReplicatedRemove.Broadcast(Entry.EquipmentInstance);
		}
	}
}

void FDungeonRealmsEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FDungeonRealmsAppliedEquipmentEntry& Entry = Entries[Index];
		if (IsValid(Entry.EquipmentInstance))
		{
			OnEquipmentReplicatedAdd.Broadcast(Entry.EquipmentInstance);
		}
	}
}

bool FDungeonRealmsEquipmentList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
{
	return FastArrayDeltaSerialize<FDungeonRealmsAppliedEquipmentEntry, FDungeonRealmsEquipmentList>(Entries, DeltaParams, *this);
}
