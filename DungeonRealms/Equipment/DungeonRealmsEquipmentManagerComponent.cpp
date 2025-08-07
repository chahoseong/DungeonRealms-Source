#include "Equipment/DungeonRealmsEquipmentManagerComponent.h"
#include "Equipment/DungeonRealmsEquipmentDefinition.h"
#include "Equipment/DungeonRealmsEquipmentInstance.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UDungeonRealmsEquipmentManagerComponent::UDungeonRealmsEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), EquipmentList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

UDungeonRealmsEquipmentInstance* UDungeonRealmsEquipmentManagerComponent::Equip(
	TSubclassOf<UDungeonRealmsEquipmentDefinition> EquipmentDefinition)
{
	UDungeonRealmsEquipmentInstance* Result = nullptr;

	if (IsValid(EquipmentDefinition))
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (IsValid(Result))
		{
			Result->OnEquipped();
			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	
	return Result;
}

void UDungeonRealmsEquipmentManagerComponent::Unequip(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	if (!IsValid(EquipmentInstance))
	{
		return;
	}

	if (IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(EquipmentInstance);
	}
	EquipmentInstance->OnUnequipped();
	EquipmentList.RemoveEntry(EquipmentInstance);
}

UDungeonRealmsEquipmentInstance* UDungeonRealmsEquipmentManagerComponent::GetEquipmentBySlot(
	const FGameplayTag& SlotTag)
{
	for (const FDungeonRealmsAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		const UDungeonRealmsEquipmentDefinition* EquipmentDefault = GetDefault<UDungeonRealmsEquipmentDefinition>(Entry.EquipmentInstance->GetEquipmentDefinition());
		if (SlotTag == EquipmentDefault->SlotTag)
		{
			return Entry.EquipmentInstance;
		}
	}

	return nullptr;
}

void UDungeonRealmsEquipmentManagerComponent::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

bool UDungeonRealmsEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool Result = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FDungeonRealmsAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (IsValid(Entry.EquipmentInstance))
		{
			Result |= Channel->ReplicateSubobject(Entry.EquipmentInstance, *Bunch, *RepFlags);
		}
	}

	return Result;
}

void UDungeonRealmsEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (!IsUsingRegisteredSubObjectList())
	{
		return;
	}

	for (const FDungeonRealmsAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (IsValid(Entry.EquipmentInstance))
		{
			AddReplicatedSubObject(Entry.EquipmentInstance);
		}
	}
}

void UDungeonRealmsEquipmentManagerComponent::OnRegister()
{
	Super::OnRegister();

	EquipmentList.OnEquipmentReplicatedAdd.AddUObject(this, &ThisClass::OnEquipmentReplicatedAdd);
	EquipmentList.OnEquipmentReplicatedRemove.AddUObject(this, &ThisClass::OnEquipmentReplicatedRemove);
}

void UDungeonRealmsEquipmentManagerComponent::OnEquipmentReplicatedAdd(
	UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	EquipmentInstance->OnEquipped();
}

void UDungeonRealmsEquipmentManagerComponent::OnEquipmentReplicatedRemove(
	UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	EquipmentInstance->OnUnequipped();
}
