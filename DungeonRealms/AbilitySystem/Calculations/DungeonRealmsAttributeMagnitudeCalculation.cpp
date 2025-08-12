#include "AbilitySystem/Calculations/DungeonRealmsAttributeMagnitudeCalculation.h"
#include "AbilitySystem/DungeonRealmsAttributeSet.h"

UDungeonRealmsAttributeMagnitudeCalculation::UDungeonRealmsAttributeMagnitudeCalculation()
{
}

float UDungeonRealmsAttributeMagnitudeCalculation::CalculateBaseMagnitude_Implementation(
	const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Total = 0.0f;
	for (const auto& AttributeToCapture : RelevantAttributesToCapture)
	{
		float Magnitude = 0.0f;
		GetCapturedAttributeMagnitude(AttributeToCapture, Spec, EvaluationParameters, Magnitude);
		Magnitude = FMath::Max(Magnitude, 0.0f);
		Total += Magnitude * CapturedAttributeToCoefficients[AttributeToCapture.AttributeToCapture];
	}
	
	const float BaseAttributeMagnitude = GetSetByCallerMagnitudeByTag(Spec, AttributeTag);
	return BaseAttributeMagnitude + Total;
}
