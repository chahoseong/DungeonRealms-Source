#include "Equipment/DungeonRealmsEquipmentActor.h"

ADungeonRealmsEquipmentActor::ADungeonRealmsEquipmentActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
}
