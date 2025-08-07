#pragma once

#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "DungeonRealmsAnimInstance.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UDungeonRealmsAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
	
	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1.0f;
};
