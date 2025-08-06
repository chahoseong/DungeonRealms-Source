#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DungeonRealmsPlayerController.generated.h"

class UDungeonRealmsInputConfig;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADungeonRealmsPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UDungeonRealmsInputConfig> InputConfig;
};
