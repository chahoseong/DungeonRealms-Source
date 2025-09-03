#include "AbilitySystem/DungeonRealmsAbilitySystemBlueprintLibrary.h"

void UDungeonRealmsAbilitySystemBlueprintLibrary::EffectContextSetDamageImpact(
	FGameplayEffectContextHandle EffectContext, float Value)
{
	if (FDungeonRealmsGameplayEffectContext* DungeonRealmsEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(EffectContext))
	{
		DungeonRealmsEffectContext->SetDamageImpact(Value);
	}
}

float UDungeonRealmsAbilitySystemBlueprintLibrary::EffectContextGetDamageImpact(
	FGameplayEffectContextHandle EffectContext)
{
	if (FDungeonRealmsGameplayEffectContext* DungeonRealmsEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(EffectContext))
	{
		return DungeonRealmsEffectContext->GetDamageImpact();
	}
	return 0.0f;
}

void UDungeonRealmsAbilitySystemBlueprintLibrary::EffectContextSetKnockbackPower(
	FGameplayEffectContextHandle EffectContext, float Value)
{
	if (FDungeonRealmsGameplayEffectContext* DungeonRealmsEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(EffectContext))
	{
		DungeonRealmsEffectContext->SetKnockbackPower(Value);
	}
}

float UDungeonRealmsAbilitySystemBlueprintLibrary::EffectContextGetKnockbackPower(
	FGameplayEffectContextHandle EffectContext)
{
	if (FDungeonRealmsGameplayEffectContext* DungeonRealmsEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(EffectContext))
	{
		return DungeonRealmsEffectContext->GetKnockbackPower();
	}
	return 0.0f;
}

void UDungeonRealmsAbilitySystemBlueprintLibrary::EffectContextSetAttackBlocked(
	FGameplayEffectContextHandle EffectContext, bool bValue)
{
	if (FDungeonRealmsGameplayEffectContext* DungeonRealmsEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(EffectContext))
	{
		DungeonRealmsEffectContext->SetAttackBlocked(bValue);
	}
}

bool UDungeonRealmsAbilitySystemBlueprintLibrary::EffectContextIsAttackBlocked(
	FGameplayEffectContextHandle EffectContext)
{
	if (FDungeonRealmsGameplayEffectContext* DungeonRealmsEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(EffectContext))
	{
		return DungeonRealmsEffectContext->IsAttackBlocked();
	}
	return false;
}

void UDungeonRealmsAbilitySystemBlueprintLibrary::EffectContextSetKnockdown(FGameplayEffectContextHandle EffectContext,
                                                                            bool bValue)
{
	if (FDungeonRealmsGameplayEffectContext* DungeonRealmsEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(EffectContext))
	{
		DungeonRealmsEffectContext->SetKnockdown(bValue);
	}
}

bool UDungeonRealmsAbilitySystemBlueprintLibrary::EffectContextShouldKnockdown(FGameplayEffectContextHandle EffectContext)
{
	if (FDungeonRealmsGameplayEffectContext* DungeonRealmsEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(EffectContext))
	{
		return DungeonRealmsEffectContext->ShouldKnockdown();
	}
	return false;
}
