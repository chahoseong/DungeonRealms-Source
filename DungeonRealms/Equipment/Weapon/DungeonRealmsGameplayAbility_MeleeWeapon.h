#pragma once

#include "CoreMinimal.h"
#include "Equipment/DungeonRealmsGameplayAbility_FromEquipment.h"
#include "DungeonRealmsGameplayAbility_MeleeWeapon.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsGameplayAbility_MeleeWeapon : public UDungeonRealmsGameplayAbility_FromEquipment
{
	GENERATED_BODY()

public:
	UDungeonRealmsGameplayAbility_MeleeWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void CommitCombo();

	UFUNCTION(BlueprintCallable)
	void OpenComboWindow();
	
	UFUNCTION(BlueprintCallable)
	void CloseComboWindow();

	UFUNCTION(BlueprintPure)
	bool IsComboWindowOpen() const;

private:
	UFUNCTION(Server, Reliable)
	void ServerCommitCombo(FGameplayAbilitySpecHandle PredicatedAbilitySpecHandle);

	bool IsCorrectPrediction(FGameplayAbilitySpecHandle PredictedAbilitySpecHandle) const;
	void AdvanceCombo(FGameplayAbilitySpecHandle AbilitySpecHandle);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> NextAttackClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

private:
	float ComboWindowCloseTime = 0.0f;
	float AllowedComboWindowTolerance = 0.15f;
	bool bComboWindowOpen = false;
	bool bWasComboCommitted = false;
};
