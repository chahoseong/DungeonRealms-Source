#include "UI/Widget/DungeonRealmsUserWidget.h"

void UDungeonRealmsUserWidget::SetController(UObject* NewController)
{
	Controller = NewController;
	OnControllerSet(NewController);
}
