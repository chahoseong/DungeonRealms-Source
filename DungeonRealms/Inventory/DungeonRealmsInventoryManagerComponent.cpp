#include "Inventory/DungeonRealmsInventoryManagerComponent.h"

#include "Net/UnrealNetwork.h"

UDungeonRealmsInventoryManagerComponent::UDungeonRealmsInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDungeonRealmsInventoryManagerComponent::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UDungeonRealmsInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDungeonRealmsInventoryManagerComponent::AddItem(UDungeonRealmsInventoryItemInstance* Item)
{
}

void UDungeonRealmsInventoryManagerComponent::RemoveItem(UDungeonRealmsInventoryItemInstance* Item)
{
}

bool UDungeonRealmsInventoryManagerComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	return Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
}

void UDungeonRealmsInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
}
