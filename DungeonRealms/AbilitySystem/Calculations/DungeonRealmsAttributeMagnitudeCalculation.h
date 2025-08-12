#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DungeonRealmsAttributeMagnitudeCalculation.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsAttributeMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UDungeonRealmsAttributeMagnitudeCalculation();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	UPROPERTY(EditDefaultsOnly, meta=(Categories="SetByCaller"))
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayAttribute, float> CapturedAttributeToCoefficients;
};
