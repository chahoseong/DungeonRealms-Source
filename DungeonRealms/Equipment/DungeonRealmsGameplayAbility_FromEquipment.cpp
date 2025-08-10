#include "Equipment/DungeonRealmsGameplayAbility_FromEquipment.h"
#include "Equipment/DungeonRealmsEquipmentInstance.h"

UDungeonRealmsEquipmentInstance* UDungeonRealmsGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec())
	{
		return Cast<UDungeonRealmsEquipmentInstance>(AbilitySpec->SourceObject);
	}
	return nullptr;
}
