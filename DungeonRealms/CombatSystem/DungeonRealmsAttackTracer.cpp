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
}

TArray<FHitResult> UDungeonRealmsAttackTracer::PerformTrace()
{
	TArray<FHitResult> TotalHits;
	
	for (int32 i = 0; i < Substeps; ++i)
	{
		TArray<FHitResult> SubHits = PerformSubTrace(
			i,
			FCollisionObjectQueryParams(ObjectTypes),
			MakeCollisionQueryParams()
		);

		for (const FHitResult& Hit : SubHits)
		{
			IgnoreActors.Add(Hit.GetActor());
		}

		TotalHits.Append(SubHits);
	}

	for (const TWeakObjectPtr Actor : IgnoreActors)
	{
		if (Actor.IsValid())
		{
			const bool bIsServer = GetCombatSystem()->GetOwner()->HasAuthority();
			const FString Label = FString::Printf(TEXT("[%s]"), bIsServer ? TEXT("Server") : TEXT("Client"));
			UE_LOG(LogTemp, Warning, TEXT("%s Ignore Actor: %s"), *Label, *Actor->GetActorNameOrLabel());	
		}
	}
	
	TotalHits = FilterBlockedHitsFromObstacle(TotalHits);
	LastHitboxTransform = HitboxComponent->GetComponentTransform();
	
	return TotalHits;
}

FCollisionQueryParams UDungeonRealmsAttackTracer::MakeCollisionQueryParams() const
{
	FCollisionQueryParams CollisionQueryParams(GetTraceTag(), GetStatId(), false);
	CollisionQueryParams.AddIgnoredActor(GetCombatSystem()->GetOwner());
	CollisionQueryParams.AddIgnoredActor(HitboxComponent->GetOwner());
	for (const TWeakObjectPtr Actor : IgnoreActors)
	{
		if (Actor.IsValid())
		{
			CollisionQueryParams.AddIgnoredActor(Actor.Get());
		}
	}
	return CollisionQueryParams;
}

TArray<FHitResult> UDungeonRealmsAttackTracer::PerformSubTrace(int32 Step,
	FCollisionObjectQueryParams ObjectQueryParams, FCollisionQueryParams CollisionQueryParams)
{
	return TArray<FHitResult>();
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
	IgnoreActors.Reset();
	HitboxComponent.Reset();
	LastHitboxTransform = FTransform::Identity;
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

UDungeonRealmsCombatSystemComponent* UDungeonRealmsAttackTracer::GetCombatSystem() const
{
	return Cast<UDungeonRealmsCombatSystemComponent>(GetOuter());
}
