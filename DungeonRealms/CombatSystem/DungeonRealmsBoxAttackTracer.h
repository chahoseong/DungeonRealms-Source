#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsAttackTracer.h"
#include "DungeonRealmsBoxAttackTracer.generated.h"

UCLASS(Blueprintable, BlueprintType)
class DUNGEONREALMS_API UDungeonRealmsBoxAttackTracer : public UDungeonRealmsAttackTracer
{
	GENERATED_BODY()

protected:
	virtual void PerformTrace(TArray<FHitResult>& OutHits) override;
};
