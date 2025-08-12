#include "CombatSystem/DungeonRealmsAttackTracer.h"
#include "CombatSystem/DungeonRealmsCombatSystemComponent.h"
#include "CombatSystem/DungeonRealmsCombatStatics.h"

void UDungeonRealmsAttackTracer::BeginTrace(UPrimitiveComponent* InHitboxComponent)
{
	if (!IsValid(InHitboxComponent))
	{
		return;
	}
	
	HitboxComponent = InHitboxComponent;
	LastHitboxTransform = HitboxComponent->GetComponentTransform();

	CollisionQueryParams = FCollisionQueryParams(GetTraceTag(), GetStatId(), false);
	CollisionQueryParams.AddIgnoredActor(GetCombatSystem()->GetOwner());
	CollisionQueryParams.AddIgnoredActor(HitboxComponent->GetOwner());
}

TArray<FHitResult> UDungeonRealmsAttackTracer::PerformTrace()
{
	TArray<FHitResult> Hits;
	PerformTrace(Hits);
	Hits = FilterBlockedHitsFromObstacle(Hits);
	LastHitboxTransform = HitboxComponent->GetComponentTransform();
	return Hits;
}

void UDungeonRealmsAttackTracer::PerformTrace(TArray<FHitResult>& OutHits)
{
}

TArray<FHitResult> UDungeonRealmsAttackTracer::FilterBlockedHitsFromObstacle(const TArray<FHitResult>& Hits) const
{
	TArray<FHitResult> Filtered;

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		bool bBlocked = UDungeonRealmsCombatStatics::HasObstacleBetween(
			HitboxComponent->GetOwner(),
			HitActor
		);
		if (!bBlocked)
		{
			Filtered.Add(Hit);
		}
	}

	return Filtered;
}

void UDungeonRealmsAttackTracer::EndTrace()
{
	HitboxComponent.Reset();
	LastHitboxTransform = FTransform::Identity;
	CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
}

AActor* UDungeonRealmsAttackTracer::GetSourceActor() const
{
	return HitboxComponent.IsValid() ? HitboxComponent->GetOwner() : nullptr;
}

FName UDungeonRealmsAttackTracer::GetTraceTag() const
{
	return SCENE_QUERY_STAT_NAME_ONLY(AttackBoxTracer_PerformTrace);
}

TStatId UDungeonRealmsAttackTracer::GetStatId() const
{
	return SCENE_QUERY_STAT_ONLY(AttackBoxTracer_PerformTrace);
}

const FTransform& UDungeonRealmsAttackTracer::GetLastHitboxTransform() const
{
	return LastHitboxTransform;
}

FCollisionObjectQueryParams UDungeonRealmsAttackTracer::GetObjectQueryParams() const
{
	static FCollisionObjectQueryParams ObjectQueryParams(ObjectTypes);
	return ObjectQueryParams;
}

const FCollisionQueryParams& UDungeonRealmsAttackTracer::GetCollisionQueryParams() const
{
	return CollisionQueryParams;
}

FCollisionQueryParams& UDungeonRealmsAttackTracer::GetCollisionQueryParams()
{
	return CollisionQueryParams;
}

UDungeonRealmsCombatSystemComponent* UDungeonRealmsAttackTracer::GetCombatSystem() const
{
	return Cast<UDungeonRealmsCombatSystemComponent>(GetOuter());
}
