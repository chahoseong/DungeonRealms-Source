#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DungeonRealmsHUD.generated.h"

class UDungeonRealmsUserWidget;
class UDungeonRealmsOverlayController;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay();
	UDungeonRealmsOverlayController* GetOverlayController() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDungeonRealmsUserWidget> ActiveOverlayWidget;
	
private:
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UDungeonRealmsUserWidget> OverlayWidgetClass;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UDungeonRealmsOverlayController> OverlayControllerClass;

	UPROPERTY()
	mutable TObjectPtr<UDungeonRealmsOverlayController> CachedOverlayController;
};
