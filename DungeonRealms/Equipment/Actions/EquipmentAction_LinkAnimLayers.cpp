#include "Equipment/Actions/EquipmentAction_LinkAnimLayers.h"
#include "Equipment/DungeonRealmsEquipmentInstance.h"
#include "GameFramework/Character.h"

void UEquipmentAction_LinkAnimLayers::OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	if (!EquipmentInstance->IsLocallyControlled())
	{
		return;
	}

	const ACharacter* OwningCharacter =
		Cast<ACharacter>(EquipmentInstance->GetPawn());
	if (IsValid(OwningCharacter))
	{
		OwningCharacter->GetMesh()->LinkAnimClassLayers(AnimLayerToLink);
	}
}

void UEquipmentAction_LinkAnimLayers::OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	if (!EquipmentInstance->IsLocallyControlled())
	{
		return;
	}

	const ACharacter* OwningCharacter =
		Cast<ACharacter>(EquipmentInstance->GetPawn());
	if (IsValid(OwningCharacter))
	{
		OwningCharacter->GetMesh()->UnlinkAnimClassLayers(AnimLayerToLink);
	}
}
