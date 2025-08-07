#include "Equipment/Features/EquipmentFeature_LinkAnimLayers.h"
#include "Equipment/DungeonRealmsEquipmentInstance.h"
#include "GameFramework/Character.h"

void UEquipmentFeature_LinkAnimLayers::OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
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

void UEquipmentFeature_LinkAnimLayers::OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
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
