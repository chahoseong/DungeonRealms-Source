#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsWidgetController.h"
#include "DungeonRealmsOverlayController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChangedSignature, float, NewValue);

class UDungeonRealmsAttributeSet;

UCLASS(BlueprintType, Blueprintable)
class DUNGEONREALMS_API UDungeonRealmsOverlayController : public UDungeonRealmsWidgetController
{
	GENERATED_BODY()

public:
	virtual void Initialize(APlayerController* NewPlayerController) override;
	virtual void BroadcastInitialValues() const override;
	
	UPROPERTY(BlueprintAssignable, Category="Resource Attributes")
	FAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Resource Attributes")
	FAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Resource Attributes")
	FAttributeChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="Resource Attributes")
	FAttributeChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="Resource Attributes")
	FAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="Resource Attributes")
	FAttributeChangedSignature OnMaxManaChanged;
	
private:
	UDungeonRealmsAttributeSet* GetAttributeSet() const;
};
