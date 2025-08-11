#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsCharacter.h"
#include "DungeonRealmsEnemyCharacter.generated.h"

class UDungeonRealmsCombatSystemComponent;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsEnemyCharacter : public ADungeonRealmsCharacter
{
	GENERATED_BODY()

public:
	ADungeonRealmsEnemyCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDungeonRealmsCombatSystemComponent> CombatSystemComponent;
};
