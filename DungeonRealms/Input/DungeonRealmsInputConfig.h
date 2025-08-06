#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "DungeonRealmsInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FDungeonRealmsInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Input"))
	FGameplayTag InputTag;
};

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UDungeonRealmsInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="DungeonRealms|Pawn")
	const UInputAction* FindNativeInputActionWithTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category="DungeonRealms|Pawn")
	const UInputAction* FindAbilityInputActionWithTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputAction"))
	TArray<FDungeonRealmsInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputAction"))
	TArray<FDungeonRealmsInputAction> AbilityInputActions;
};
