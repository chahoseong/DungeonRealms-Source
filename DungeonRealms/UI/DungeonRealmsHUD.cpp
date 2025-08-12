#include "UI/DungeonRealmsHUD.h"
#include "UI/Controllers/DungeonRealmsOverlayController.h"
#include "UI/Widget/DungeonRealmsUserWidget.h"

void ADungeonRealmsHUD::InitOverlay()
{
	ActiveOverlayWidget = CreateWidget<UDungeonRealmsUserWidget>(GetOwningPlayerController(), OverlayWidgetClass);
	ActiveOverlayWidget->AddToViewport();

	UDungeonRealmsOverlayController* OverlayController = GetOverlayController();
	ActiveOverlayWidget->SetController(OverlayController);
	OverlayController->BroadcastInitialValues();
}

UDungeonRealmsOverlayController* ADungeonRealmsHUD::GetOverlayController() const
{
	if (!CachedOverlayController)
	{
		CachedOverlayController = NewObject<UDungeonRealmsOverlayController>(
			const_cast<ADungeonRealmsHUD*>(this),
			OverlayControllerClass
		);
		APlayerController* PlayerController = GetOwningPlayerController();
		CachedOverlayController->Initialize(PlayerController);
	}

	return CachedOverlayController;
}
