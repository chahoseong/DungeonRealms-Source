#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DungeonRealmsGameMode.generated.h"

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	int32 NextPlayerIndex = 0;
};
