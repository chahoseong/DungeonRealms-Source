#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "DungeonRealmsAttributeDefinition.generated.h"

USTRUCT(BlueprintType)
struct FDungeonRealmsAttributeDefinition : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Id;

	UPROPERTY(EditAnywhere, Category="Primary Attributes")
	float Strength = 0.0f;

	UPROPERTY(EditAnywhere, Category="Primary Attributes")
	float Intelligence = 0.0f;

	UPROPERTY(EditAnywhere, Category="Primary Attributes")
	float Dexterity = 0.0f;

	UPROPERTY(EditAnywhere, Category="Primary Attributes")
	float Will = 0.0f;

	UPROPERTY(EditAnywhere, Category="Secondary Attributes")
	float AttackDamage = 0.0f;

	UPROPERTY(EditAnywhere, Category="Secondary Attributes")
	float AbilityPower = 0.0f;

	UPROPERTY(EditAnywhere, Category="Secondary Attributes")
	float Armor = 0.0f;

	UPROPERTY(EditAnywhere, Category="Secondary Attributes")
	float Resistance = 0.0f;

	UPROPERTY(EditAnywhere, Category="Secondary Attributes")
	float MaxHealth = 0.0f;

	UPROPERTY(EditAnywhere, Category="Secondary Attributes")
	float MaxStamina = 0.0f;

	UPROPERTY(EditAnywhere, Category="Secondary Attributes")
	float MaxMana = 0.0f;

	UPROPERTY(EditAnywhere, Category="Secondary Attributes")
	float MaxPoise = 0.0f;

	TMap<FGameplayTag, float> GetPrimaryAttributeMagnitudes() const;
	TMap<FGameplayTag, float> GetSecondaryAttributeMagnitudes() const;
};
