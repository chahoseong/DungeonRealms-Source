#include "UI/Controllers/DungeonRealmsWidgetController.h"

void UDungeonRealmsWidgetController::Initialize(APlayerController* NewPlayerController)
{
	PlayerController = NewPlayerController;
}

void UDungeonRealmsWidgetController::BroadcastInitialValues() const
{
}

APlayerController* UDungeonRealmsWidgetController::GetPlayerController() const
{
	return PlayerController.IsValid() ? PlayerController.Get() : nullptr;
}

APlayerState* UDungeonRealmsWidgetController::GetPlayerState() const
{
	return PlayerController.IsValid()
		       ? PlayerController->GetPlayerState<APlayerState>()
		       : nullptr;
}
