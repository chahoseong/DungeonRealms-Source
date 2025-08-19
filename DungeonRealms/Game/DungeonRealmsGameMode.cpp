#include "Game/DungeonRealmsGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "Player/DungeonRealmsPlayerController.h"

void ADungeonRealmsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ADungeonRealmsPlayerController* DungeonRealmsPlayerController = CastChecked<ADungeonRealmsPlayerController>(NewPlayer);
	DungeonRealmsPlayerController->SetPlayerIndex(NextPlayerIndex);
	++NextPlayerIndex;
}
