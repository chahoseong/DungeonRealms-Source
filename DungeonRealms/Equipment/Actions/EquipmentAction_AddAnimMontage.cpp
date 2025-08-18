#include "Equipment/Actions/EquipmentAction_AddAnimMontage.h"
#include "Equipment/DungeonRealmsEquipmentInstance.h"
#include "Characters/DungeonRealmsCharacter.h"

void UEquipmentAction_AddAnimMontage::OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	if (ADungeonRealmsCharacter* DungeonRealmsCharacter = EquipmentInstance->GetPawn<ADungeonRealmsCharacter>())
	{
		for (const auto& Pair : AnimMontages)
		{
			DungeonRealmsCharacter->AddAnimMontage(Pair.Key, Pair.Value);
		}
	}
}

void UEquipmentAction_AddAnimMontage::OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	if (ADungeonRealmsCharacter* DungeonRealmsCharacter = EquipmentInstance->GetPawn<ADungeonRealmsCharacter>())
	{
		for (const auto& Pair : AnimMontages)
		{
			DungeonRealmsCharacter->RemoveAnimMontage(Pair.Key);
		}
	}
}
