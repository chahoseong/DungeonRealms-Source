#include "Equipment/DungeonRealmsEquipmentManagerComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/DungeonRealmsAttributeDefinition.h"
#include "Characters/DungeonRealmsCharacter.h"
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
			ApplyEquipEffect(Result);
		}
	}
	
	return Result;
}

void UDungeonRealmsEquipmentManagerComponent::Unequip(UDungeonRealmsEquipmentInstance* EquipmentInstance, bool bForReplacement)
{
	if (!IsValid(EquipmentInstance))
	{
		return;
	}

	RemoveEquipEffect(EquipmentInstance);
	if (IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(EquipmentInstance);
	}
	EquipmentInstance->OnUnequipped();
	EquipmentList.RemoveEntry(EquipmentInstance);

	if (!bForReplacement)
	{
		for (const FGameplayTag& SlotTag : EquipmentInstance->GetSlotTags())
		{
			if (const TSubclassOf DefaultEquipment = DefaultEquipments.FindRef(SlotTag))
			{
				Equip(DefaultEquipment);
			}
		}
	}
}

UDungeonRealmsEquipmentInstance* UDungeonRealmsEquipmentManagerComponent::GetEquipmentBySlot(
	const FGameplayTag& SlotTag)
{
	for (const FDungeonRealmsAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		const UDungeonRealmsEquipmentDefinition* EquipmentDefault = GetDefault<UDungeonRealmsEquipmentDefinition>(Entry.EquipmentInstance->GetEquipmentDefinition());
		if (EquipmentDefault->SlotTags.HasTagExact(SlotTag))
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

void UDungeonRealmsEquipmentManagerComponent::ApplyEquipEffect(const UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	ADungeonRealmsCharacter* OwningCharacter = GetOwner<ADungeonRealmsCharacter>();
	if (!IsValid(OwningCharacter))
	{
		return;
	}

	TMap<FGameplayTag, float> AttributeBonuses;
	for (const FEquipmentAttributeBonus& Bonus : EquipmentInstance->GetAttributeBonuses())
	{
		AttributeBonuses.Add(Bonus.AttributeTag, Bonus.Magnitude);
	}

	if (AttributeBonuses.Num() > 0)
	{
		FActiveGameplayEffectHandle ActiveEffectHandle = OwningCharacter->ApplyEffectToSelf(
			EquipEffectClass, AttributeBonuses);
		ActiveEquipEffectHandles.Add(EquipmentInstance->GetEquipmentId(), ActiveEffectHandle);
	}
}

void UDungeonRealmsEquipmentManagerComponent::RemoveEquipEffect(const UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	ADungeonRealmsCharacter* OwningCharacter = GetOwner<ADungeonRealmsCharacter>();
	if (!IsValid(OwningCharacter))
	{
		return;
	}

	FActiveGameplayEffectHandle ActiveEffectHandle;
	ActiveEquipEffectHandles.RemoveAndCopyValue(EquipmentInstance->GetEquipmentId(), ActiveEffectHandle);
	if (ActiveEffectHandle.IsValid())
	{
		OwningCharacter->RemoveActiveEffect(ActiveEffectHandle);
	}
}
