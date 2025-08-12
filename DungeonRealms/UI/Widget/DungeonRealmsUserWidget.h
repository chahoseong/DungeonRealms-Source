#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DungeonRealmsUserWidget.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetController(UObject* NewController);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Controller Set"))
	void OnControllerSet(UObject* NewController);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> Controller;
};
