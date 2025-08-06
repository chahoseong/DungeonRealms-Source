#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DungeonRealmsCharacter.generated.h"

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADungeonRealmsCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
};
