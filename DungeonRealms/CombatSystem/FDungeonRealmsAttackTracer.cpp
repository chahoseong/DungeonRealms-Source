#include "CombatSystem/FDungeonRealmsAttackTracer.h"
#include "CombatSystem/DungeonRealmsCombatStatics.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

FDungeonRealmsAttackTracer::FDungeonRealmsAttackTracer()
{
	AttackTraceShapes.Add(UBoxComponent::StaticClass(),
		CreateAttackTraceShape<FBoxAttackTrace>());
	AttackTraceShapes.Add(UCapsuleComponent::StaticClass(),
		CreateAttackTraceShape<FCapsuleAttackTrace>());
}

void FDungeonRealmsAttackTracer::BeginTrace(UPrimitiveComponent* InHitboxComponent,
                                           const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
                                           const TArray<AActor*>& IgnoreActors)
{
	if (!IsValid(InHitboxComponent))
	{
		return;
	}
	HitboxComponent = InHitboxComponent;
	ObjectQueryParams = FCollisionObjectQueryParams(ObjectTypes);
	CollisionQueryParams = FCollisionQueryParams(
		SCENE_QUERY_STAT(DungeonRealmsAttackTracer_PerformTrace),
		false
	);
	CollisionQueryParams.AddIgnoredActors(IgnoreActors);
	LastHitboxTransform = HitboxComponent->GetComponentTransform();
}

TArray<FHitResult> FDungeonRealmsAttackTracer::PerformTrace(int32 Substeps,
	bool bDrawDebug)
{
	TArray<FHitResult> TotalHits;
	
	if (HitboxComponent.IsValid())
	{
		FAttackTraceShape AttackTraceShape = AttackTraceShapes.FindRef(HitboxComponent->GetClass(),
			CreateAttackTraceShape<FEmptyAttackTrace>());
		
		for (int32 i = 0; i < Substeps; ++i)
		{
			FTransform TraceStartTransform = UKismetMathLibrary::TLerp(
				GetLastHitboxTransform(),
				HitboxComponent->GetComponentTransform(),
				static_cast<float>(i) / Substeps,
				ELerpInterpolationMode::DualQuatInterp
			);
			
			FTransform TraceEndTransform = UKismetMathLibrary::TLerp(
				GetLastHitboxTransform(),
				HitboxComponent->GetComponentTransform(),
				static_cast<float>(i + 1) / Substeps,
				ELerpInterpolationMode::DualQuatInterp
			);
			
			FTransform TraceAverageTransform = UKismetMathLibrary::TLerp(
				TraceStartTransform,
				TraceEndTransform,
				0.5f,
				ELerpInterpolationMode::DualQuatInterp
			);
			
			TArray<FHitResult> SubHits = Visit(
				FPerformAttackTrace(
					HitboxComponent.Get(),
					TraceStartTransform.GetLocation(),
					TraceEndTransform.GetLocation(),
					TraceAverageTransform.GetRotation(),
					ObjectQueryParams,
					CollisionQueryParams
				),
				AttackTraceShape);

			if (bDrawDebug && GetSourceActor()->HasAuthority())
			{
				Visit(
					FDrawDebugAttackTrace(
						HitboxComponent.Get(),
						TraceAverageTransform.GetLocation(),
						TraceAverageTransform.GetRotation(),
						SubHits.Num() > 0 ? FColor::Red : FColor::Green,
						false,
						2.0f
					),
					AttackTraceShape
				);
			}
			
			for (const FHitResult& Hit : SubHits)
			{
				CollisionQueryParams.AddIgnoredActor(Hit.GetActor());
			}

			TotalHits.Append(SubHits);
		}
		
		TotalHits = FilterBlockedHitsFromObstacle(TotalHits);
	}
	
	LastHitboxTransform = HitboxComponent->GetComponentTransform();
	
	return TotalHits;
}

TArray<FHitResult> FDungeonRealmsAttackTracer::FilterBlockedHitsFromObstacle(const TArray<FHitResult>& Hits) const
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

void FDungeonRealmsAttackTracer::EndTrace()
{
	ObjectQueryParams = FCollisionObjectQueryParams::DefaultObjectQueryParam;
	CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
	HitboxComponent.Reset();
}

AActor* FDungeonRealmsAttackTracer::GetSourceActor() const
{
	return HitboxComponent.IsValid() ? HitboxComponent->GetOwner() : nullptr;
}

const FTransform& FDungeonRealmsAttackTracer::GetLastHitboxTransform() const
{
	return LastHitboxTransform;
}
