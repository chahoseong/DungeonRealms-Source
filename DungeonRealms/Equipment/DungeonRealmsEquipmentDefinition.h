#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "DungeonRealmsEquipmentDefinition.generated.h"

class UDungeonRealmsEquipmentInstance;

USTRUCT(BlueprintType)
struct FDungeonRealmsEquipmentId
{
	GENERATED_BODY()

	static constexpr int32 INVALID_EQUIPMENT_ID = -1;
	
	FDungeonRealmsEquipmentId(int32 InEquipmentId = INVALID_EQUIPMENT_ID) : EquipmentId(InEquipmentId) { }
	operator int32() const { return EquipmentId; }
	int32 ToInt() const { return EquipmentId; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	int32 EquipmentId = INVALID_EQUIPMENT_ID;
};

USTRUCT(BlueprintType)
struct FEquipmentAttributeBonus
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute Attribute;

	UPROPERTY(EditDefaultsOnly)
	float Magnitude;
};

UCLASS(Abstract, Const, Blueprintable, BlueprintType)
class DUNGEONREALMS_API UDungeonRealmsEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UDungeonRealmsEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	FDungeonRealmsEquipmentId EquipmentId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment", meta=(Categories="Equipment.Slot"))
	FGameplayTagContainer SlotTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	TArray<FEquipmentAttributeBonus> AttributeBonuses;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	TSubclassOf<UDungeonRealmsEquipmentInstance> EquipmentInstanceClass;
};
