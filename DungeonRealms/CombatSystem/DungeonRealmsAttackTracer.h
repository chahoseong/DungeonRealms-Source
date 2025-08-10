#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DungeonRealmsAttackTracer.generated.h"

class UDungeonRealmsCombatSystemComponent;

UCLASS(Abstract, Blueprintable, BlueprintType)
class DUNGEONREALMS_API UDungeonRealmsAttackTracer : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginTrace(UPrimitiveComponent* InHitboxComponent);
	TArray<FHitResult> PerformTrace();
	virtual void EndTrace();
	
	AActor* GetSourceActor() const;

	template <typename T> requires std::is_base_of_v<UPrimitiveComponent, T>
	T* GetHitboxComponent() const;

protected:
	virtual void PerformTrace(TArray<FHitResult>& OutHits);

	const FTransform& GetLastHitboxTransform() const;
	FCollisionObjectQueryParams GetObjectQueryParams() const;

	UDungeonRealmsCombatSystemComponent* GetCombatSystem() const;

private:
	TArray<FHitResult> FilterBlockedHitsFromObstacle(const TArray<FHitResult>& Hits) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Substeps = 2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDrawDebug = false;

private:
	TWeakObjectPtr<USceneComponent> HitboxComponent;
	FTransform LastHitboxTransform;
};

template <typename T> requires std::is_base_of_v<UPrimitiveComponent, T>
T* UDungeonRealmsAttackTracer::GetHitboxComponent() const
{
	return HitboxComponent.IsValid() ? Cast<T>(HitboxComponent) : nullptr;
}
