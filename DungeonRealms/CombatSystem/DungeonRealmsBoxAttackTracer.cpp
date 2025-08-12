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

void UDungeonRealmsBoxAttackTracer::PerformTrace(TArray<FHitResult>& OutHits)
{
	UBoxComponent* BoxComponent = GetHitboxComponent<UBoxComponent>();
	
	for (int32 i = 0; i < Substeps; ++i)
	{
		FTransform TraceStartTransform = UKismetMathLibrary::TLerp(
			GetLastHitboxTransform(),
			BoxComponent->GetComponentTransform(),
			static_cast<float>(i) / Substeps,
			ELerpInterpolationMode::DualQuatInterp
		);
		FTransform TraceEndTransform = UKismetMathLibrary::TLerp(
			GetLastHitboxTransform(),
			BoxComponent->GetComponentTransform(),
			static_cast<float>(i + 1) / Substeps,
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
			GetObjectQueryParams(),
			FCollisionShape::MakeBox(BoxComponent->GetScaledBoxExtent()),
			GetCollisionQueryParams()
		);

		for (const FHitResult& Hit : SubHits)
		{
			AActor* HitActor = Hit.GetActor();
			GetCollisionQueryParams().AddIgnoredActor(HitActor);
		}
		
		OutHits.Append(SubHits);

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
	}
}
