#include "Equipment/DungeonRealmsEquipmentInstance.h"
#include "Equipment/Actions/DungeonRealmsEquipmentAction.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/DungeonRealmsAbilitySet.h"
#include "Net/UnrealNetwork.h"

UDungeonRealmsEquipmentInstance::UDungeonRealmsEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDungeonRealmsEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

UWorld* UDungeonRealmsEquipmentInstance::GetWorld() const
{
	const APawn* OwningPawn = GetPawn();
	return IsValid(OwningPawn) ? OwningPawn->GetWorld() : nullptr;
}

TSubclassOf<UDungeonRealmsEquipmentDefinition> UDungeonRealmsEquipmentInstance::GetEquipmentDefinition() const
{
	return EquipmentDefinition;
}

FDungeonRealmsEquipmentId UDungeonRealmsEquipmentInstance::GetEquipmentId() const
{
	const UDungeonRealmsEquipmentDefinition* EquipmentDefault = GetDefault<UDungeonRealmsEquipmentDefinition>(EquipmentDefinition);
	return EquipmentDefault->EquipmentId;
}

const TArray<FEquipmentAttributeBonus>& UDungeonRealmsEquipmentInstance::GetAttributeBonuses() const
{
	const UDungeonRealmsEquipmentDefinition* EquipmentDefault = GetDefault<UDungeonRealmsEquipmentDefinition>(EquipmentDefinition);
	return EquipmentDefault->AttributeBonuses;
}

void UDungeonRealmsEquipmentInstance::AddEquipmentActor(AActor* EquipmentActor)
{
	SpawnedActors.Add(EquipmentActor);
}

void UDungeonRealmsEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		Actor->Destroy();
	}
	SpawnedActors.Reset();
}

APawn* UDungeonRealmsEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

bool UDungeonRealmsEquipmentInstance::HasAuthority() const
{
	const APawn* OwningPawn = GetPawn();
	return IsValid(OwningPawn) ? OwningPawn->HasAuthority() : false;
}

bool UDungeonRealmsEquipmentInstance::IsLocallyControlled() const
{
	const APawn* OwningPawn = GetPawn();
	return IsValid(OwningPawn) ? OwningPawn->IsLocallyControlled() : false;
}

void UDungeonRealmsEquipmentInstance::OnEquipped()
{
	for (UDungeonRealmsEquipmentAction* Feature : EquipmentActions)
	{
		Feature->OnEquipped(this);
	}
	K2_OnEquipped();
}

void UDungeonRealmsEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
	for (UDungeonRealmsEquipmentAction* Feature : EquipmentActions)
	{
		Feature->OnUnequipped(this);
	}
}
