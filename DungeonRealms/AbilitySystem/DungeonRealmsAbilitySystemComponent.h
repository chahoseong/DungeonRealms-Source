#pragma once

#include "AbilitySystemComponent.h"
#include "DungeonRealmsAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEONREALMS_API UDungeonRealmsAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	static UDungeonRealmsAbilitySystemComponent* FindAbilitySystemComponent(const AActor* Actor);
	
public:
	UDungeonRealmsAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();
	
	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);

protected:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
};
