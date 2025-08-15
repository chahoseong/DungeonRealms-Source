#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsGameplayAbility.h"
#include "DungeonRealmsGameplayAbility_Jump.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsGameplayAbility_Jump : public UDungeonRealmsGameplayAbility
{
	GENERATED_BODY()

public:
	UDungeonRealmsGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category="DungeonRealms|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category="DungeonRealms|Ability")
	void CharacterJumpStop();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float JumpSpeed;

private:
	float CachedJumpSpeed;
};
