#include "CombatSystem/DungeonRealmsBoxAttackTracer.h"
#include "Components/BoxComponent.h"
#include "KismetTraceUtils.h"
#include "Kismet/KismetMathLibrary.h"

FName UDungeonRealmsBoxAttackTracer::GetTraceTag() const
{
	return SCENE_QUERY_STAT_NAME_ONLY(BoxAttackTracer_PerformTrace);
}

TStatId UDungeonRealmsBoxAttackTracer::GetStatId() const
{
	return SCENE_QUERY_STAT_ONLY(BoxAttackTracer_PerformTrace);
}

TArray<FHitResult> UDungeonRealmsBoxAttackTracer::PerformSubTrace(int32 Step,
	FCollisionObjectQueryParams ObjectQueryParams, FCollisionQueryParams CollisionQueryParams)
{
	UBoxComponent* BoxComponent = GetHitboxComponent<UBoxComponent>();

	FTransform TraceStartTransform = UKismetMathLibrary::TLerp(
		GetLastHitboxTransform(),
		BoxComponent->GetComponentTransform(),
		static_cast<float>(Step) / Substeps,
		ELerpInterpolationMode::DualQuatInterp
	);
	FTransform TraceEndTransform = UKismetMathLibrary::TLerp(
		GetLastHitboxTransform(),
		BoxComponent->GetComponentTransform(),
		static_cast<float>(Step + 1) / Substeps,
		ELerpInterpolationMode::DualQuatInterp
	);
	FTransform TraceAverageTransform = UKismetMathLibrary::TLerp(
		TraceStartTransform,
		TraceEndTransform,
		0.5f,
		ELerpInterpolationMode::DualQuatInterp
	);

	TArray<FHitResult> SubHits;
	BoxComponent->GetWorld()->SweepMultiByObjectType(
		SubHits,
		TraceStartTransform.GetLocation(),
		TraceEndTransform.GetLocation(),
		TraceAverageTransform.GetRotation(),
		ObjectQueryParams,
		FCollisionShape::MakeBox(BoxComponent->GetScaledBoxExtent()),
		CollisionQueryParams
	);
	
	if (bDrawDebug && GetSourceActor()->HasAuthority())
	{
		DrawDebugSweptBox(
			GetWorld(),
			TraceStartTransform.GetLocation(),
			TraceEndTransform.GetLocation(),
			TraceAverageTransform.GetRotation().Rotator(),
			BoxComponent->GetScaledBoxExtent(),
			SubHits.Num() > 0 ? FColor::Red : FColor::Green,
			false,
			2.0f
		);
	}

	return SubHits;
}
