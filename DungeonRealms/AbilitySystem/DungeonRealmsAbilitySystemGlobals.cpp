#include "AbilitySystem/DungeonRealmsAbilitySystemGlobals.h"
#include "AbilitySystem/DungeonRealmsGameplayEffectContext.h"

UDungeonRealmsAbilitySystemGlobals::UDungeonRealmsAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* UDungeonRealmsAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FDungeonRealmsGameplayEffectContext();
}
