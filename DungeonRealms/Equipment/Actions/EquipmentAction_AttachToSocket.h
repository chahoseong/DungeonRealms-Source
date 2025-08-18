#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsEquipmentAction.h"
#include "EquipmentAction_AttachToSocket.generated.h"

USTRUCT()
struct FDungeonRealmsEquipmentActorToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Equipment")
	TSubclassOf<AActor> EquipmentActorClass;

	UPROPERTY(EditAnywhere, Category="Equipment")
	FName AttachSocketName;

	UPROPERTY(EditAnywhere, Category="Equipment")
	FTransform AttachTransform;
};

UCLASS(DisplayName="Attach To Socket")
class DUNGEONREALMS_API UEquipmentAction_AttachToSocket : public UDungeonRealmsEquipmentAction
{
	GENERATED_BODY()

public:
	virtual void OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;
	virtual void OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FDungeonRealmsEquipmentActorToSpawn> EquipmentActorsToSpawn;
};
