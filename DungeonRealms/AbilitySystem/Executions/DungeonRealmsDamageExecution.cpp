#include "AbilitySystem/Executions/DungeonRealmsDamageExecution.h"
#include "AbilitySystem/DungeonRealmsAttributeSet.h"
#include "DungeonRealmsGameplayTags.h"
#include "AbilitySystem/DungeonRealmsGameplayEffectContext.h"

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition AttackDamageDef;
	FGameplayEffectAttributeCaptureDefinition AbilityPowerDef;
	FGameplayEffectAttributeCaptureDefinition ArmorDef;
	FGameplayEffectAttributeCaptureDefinition ResistanceDef;

	TMap<FGameplayTag, FGameplayAttribute> TagsToAttributes;
	
	FDamageStatics()
	{
		AttackDamageDef = FGameplayEffectAttributeCaptureDefinition(UDungeonRealmsAttributeSet::GetAttackDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
		AbilityPowerDef = FGameplayEffectAttributeCaptureDefinition(UDungeonRealmsAttributeSet::GetAbilityPowerAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
		ArmorDef = FGameplayEffectAttributeCaptureDefinition(UDungeonRealmsAttributeSet::GetArmorAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		ResistanceDef = FGameplayEffectAttributeCaptureDefinition(UDungeonRealmsAttributeSet::GetResistanceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UDungeonRealmsDamageExecution::UDungeonRealmsDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().AbilityPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceDef);
}

void UDungeonRealmsDamageExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float SourceAttackDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().AttackDamageDef,
		EvaluationParameters,
		SourceAttackDamage
	);

	float SourceAbilityPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().AbilityPowerDef,
		EvaluationParameters,
		SourceAbilityPower
	);

	float TargetArmor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorDef,
		EvaluationParameters,
		TargetArmor
	);

	float TargetResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ResistanceDef,
		EvaluationParameters,
		TargetResistance
	);

	float TotalDamage = 0.0f;
	
	const float AttackDamageCoefficient = Spec.GetSetByCallerMagnitude(DungeonRealmsGameplayTags::SetByCaller_Damage_AttackDamage_Coefficient);
	const float AttackDamageReductionRate = 100 / (100 + TargetArmor);
	TotalDamage += SourceAttackDamage * AttackDamageCoefficient * AttackDamageReductionRate;

	const float AbilityPowerCoefficient = Spec.GetSetByCallerMagnitude(DungeonRealmsGameplayTags::SetByCaller_Damage_AbilityPower_Coefficient);
	const float AbilityPowerReductionRate = 100 / (100 + TargetResistance);
	TotalDamage += SourceAbilityPower * AbilityPowerCoefficient * AbilityPowerReductionRate;

	FDungeonRealmsGameplayEffectContext* ExtraContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(Spec.GetContext());
	if (ExtraContext->IsAttackBlocked())
	{
		TotalDamage *= 0.1f;
	}
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		UDungeonRealmsAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		TotalDamage
	));
}
