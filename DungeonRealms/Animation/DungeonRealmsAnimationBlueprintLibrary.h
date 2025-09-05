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
	static bool IsPlayInPreview(UObject* WorldContextObject);
};
