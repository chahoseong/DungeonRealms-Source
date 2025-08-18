#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "DungeonRealmsAbilitySystemGlobals.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	UDungeonRealmsAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
