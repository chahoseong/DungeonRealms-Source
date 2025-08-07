#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsEquipmentFeature.h"
#include "EquipmentFeature_AttachToSocket.generated.h"

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

UCLASS(DisplayName="AttachToSocket")
class DUNGEONREALMS_API UEquipmentFeature_AttachToSocket : public UDungeonRealmsEquipmentFeature
{
	GENERATED_BODY()

public:
	virtual void OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;
	virtual void OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FDungeonRealmsEquipmentActorToSpawn> EquipmentActorsToSpawn;
};
