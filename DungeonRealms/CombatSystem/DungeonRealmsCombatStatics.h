#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonRealmsCombatStatics.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsCombatStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool HasObstacleBetween(const AActor* SourceActor, const AActor* TargetActor);
};
