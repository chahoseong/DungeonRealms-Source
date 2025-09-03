#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonRealmsAnimationBlueprintLibrary.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsAnimationBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Dungeon Realms|Animation")
	static void AddOrUpdateWarpTargetFromLocation(AActor* Actor, const FVector& Location);
};
