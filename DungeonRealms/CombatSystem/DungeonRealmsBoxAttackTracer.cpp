#include "CombatSystem/DungeonRealmsBoxAttackTracer.h"
#include "CombatSystem/DungeonRealmsCombatSystemComponent.h"
#include "Components/BoxComponent.h"
#include "KismetTraceUtils.h"
#include "Kismet/KismetMathLibrary.h"

void UDungeonRealmsBoxAttackTracer::PerformTrace(TArray<FHitResult>& OutHits)
{
	UBoxComponent* BoxComponent = GetHitboxComponent<UBoxComponent>();
	
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(AttackBoxTracer_PerformTrace), false);;
	CollisionQueryParams.AddIgnoredActor(GetCombatSystem()->GetOwner());
	CollisionQueryParams.AddIgnoredActor(BoxComponent->GetOwner());
	
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
			CollisionQueryParams
		);

		for (const FHitResult& Hit : SubHits)
		{
			AActor* HitActor = Hit.GetActor();
			CollisionQueryParams.AddIgnoredActor(HitActor);
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
