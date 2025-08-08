#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DungeonRealmsAbilitySet.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class UGameplayAbility;

struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;

USTRUCT(BlueprintType)
struct FGameplayAbilityToGrant
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly, meta=(Categories="Input"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FGameplayEffectToApply
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> Effect;

	UPROPERTY(EditDefaultsOnly)
	float Level = 1.0f;
};

USTRUCT(BlueprintType)
struct FDungeonRealmsAbilitySetGrantedHandles
{
	GENERATED_BODY()

public:
	void AddGameplayAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);

	void TakeFromAbilitySystem(UAbilitySystemComponent* TargetAbilitySystem);

	int32 Num() const { return GameplayAbilitySpecHandles.Num() + GameplayEffectHandles.Num(); }

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GameplayAbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};

UCLASS(BlueprintType, Const)
class DUNGEONREALMS_API UDungeonRealmsAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	UDungeonRealmsAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystem,
		FDungeonRealmsAbilitySetGrantedHandles& OutHandles) const;
	void GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystem,
		UObject* SourceObject,
		FDungeonRealmsAbilitySetGrantedHandles& OutHandles
	) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FGameplayAbilityToGrant> GrantedGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effects", meta=(TitleProperty=Effect))
	TArray<FGameplayEffectToApply> AppliedGameplayEffects;
};
