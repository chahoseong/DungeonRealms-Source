#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsGameplayEffectContext.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonRealmsAbilitySystemBlueprintLibrary.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsAbilitySystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Dungeon Realms|EffectContext", meta=(DisplayName="SetDamageImpact"))
	static void EffectContextSetDamageImpact(FGameplayEffectContextHandle EffectContext, float Value);

	UFUNCTION(BlueprintPure, Category="Dungeon Realms|EffectContext", meta=(DisplayName="GetDamageImpact"))
	static float EffectContextGetDamageImpact(FGameplayEffectContextHandle EffectContext);
	
	UFUNCTION(BlueprintCallable, Category="Dungeon Realms|EffectContext", meta=(DisplayName="SetKnockbackPower"))
	static void EffectContextSetKnockbackPower(FGameplayEffectContextHandle EffectContext, float Value);

	UFUNCTION(BlueprintPure, Category="Dungeon Realms|EffectContext", meta=(DisplayName="GetKnockbackPower"))
	static float EffectContextGetKnockbackPower(FGameplayEffectContextHandle EffectContext);
	
	UFUNCTION(BlueprintCallable, Category="Dungeon Realms|EffectContext", meta=(DisplayName="SetKnockdown"))
	static void EffectContextSetKnockdown(FGameplayEffectContextHandle EffectContext, bool bValue);

	UFUNCTION(BlueprintPure, Category="Dungeon Realms|EffectContext", meta=(DisplayName="ShouldKnockdown"))
	static bool EffectContextShouldKnockdown(FGameplayEffectContextHandle EffectContext);
};
