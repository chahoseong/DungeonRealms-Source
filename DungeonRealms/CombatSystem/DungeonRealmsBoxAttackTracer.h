#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsAttackTracer.h"
#include "DungeonRealmsBoxAttackTracer.generated.h"

UCLASS(Blueprintable, BlueprintType)
class DUNGEONREALMS_API UDungeonRealmsBoxAttackTracer : public UDungeonRealmsAttackTracer
{
	GENERATED_BODY()

protected:
	virtual FName GetTraceTag() const override;
	virtual TStatId GetStatId() const override;
	
	virtual TArray<FHitResult> PerformSubTrace(
		int32 Step,
		FCollisionObjectQueryParams ObjectQueryParams,
		FCollisionQueryParams CollisionQueryParams
	) override;
};
