#pragma once

#include "Abilities/GameplayAbility.h"
#include "DungeonRealmsGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EDungeonRealmsAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnGranted,
	Manually,
};

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	static bool HasActivationPolicy(UGameplayAbility* Ability, EDungeonRealmsAbilityActivationPolicy ActivationPolicy);

public:
	UDungeonRealmsGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="InputPressed")
	void K2_InputPressed();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="InputReleased")
	void K2_InputReleased();
	
	EDungeonRealmsAbilityActivationPolicy GetActivationPolicy() const;

	UFUNCTION(BlueprintPure)
	AController* GetControllerFromActorInfo() const;

	template <typename T>
	T* GetControllerFromActorInfo() const
	{
		return Cast<T>(GetControllerFromActorInfo());
	}

	bool IsRunningOnDedicatedServer() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DungeonRealms|Ability Activation")
	EDungeonRealmsAbilityActivationPolicy ActivationPolicy = EDungeonRealmsAbilityActivationPolicy::OnInputTriggered;
};
