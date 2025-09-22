#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsAttackTraceShapes.h"

class FDungeonRealmsAttackTracer
{
public:
	FDungeonRealmsAttackTracer();
	
	void BeginTrace(
		UPrimitiveComponent* InHitboxComponent,
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		const TArray<AActor*>& IgnoreActors
	);
	TArray<FHitResult> PerformTrace(
		int32 Substeps,
		bool bDrawDebug
	);
	void EndTrace();
	
	AActor* GetSourceActor() const;

	template <typename T> requires std::is_base_of_v<UPrimitiveComponent, T>
	T* GetHitboxComponent() const;

protected:
	const FTransform& GetLastHitboxTransform() const;

private:
	TArray<FHitResult> FilterBlockedHitsFromObstacle(const TArray<FHitResult>& Hits) const;

private:
	TWeakObjectPtr<UPrimitiveComponent> HitboxComponent;
	FCollisionObjectQueryParams ObjectQueryParams;
	FCollisionQueryParams CollisionQueryParams;
	FTransform LastHitboxTransform;
	TMap<UClass*, FAttackTraceShape> AttackTraceShapes;
};

template <typename T> requires std::is_base_of_v<UPrimitiveComponent, T>
T* FDungeonRealmsAttackTracer::GetHitboxComponent() const
{
	return HitboxComponent.IsValid() ? Cast<T>(HitboxComponent) : nullptr;
}
