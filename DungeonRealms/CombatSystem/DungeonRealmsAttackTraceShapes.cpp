#include "DungeonRealmsAttackTraceShapes.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

FPerformAttackTrace::FPerformAttackTrace(UPrimitiveComponent* HitboxComponent,
	const FVector& Start, const FVector& End, const FQuat& Rot,
	const FCollisionObjectQueryParams& ObjectQueryParams,
	const FCollisionQueryParams& CollisionQueryParams)
	: HitboxComponent(HitboxComponent),
	  Start(Start), End(End), Rot(Rot),
	  ObjectQueryParams(ObjectQueryParams), CollisionQueryParams(CollisionQueryParams)
{
}

TArray<FHitResult> FPerformAttackTrace::operator()(const FEmptyAttackTrace& Empty) const
{
	return TArray<FHitResult>();
}

TArray<FHitResult> FPerformAttackTrace::operator()(const FBoxAttackTrace& Box) const
{
	TArray<FHitResult> Hits;
	if (const UBoxComponent* BoxComponent = Cast<UBoxComponent>(HitboxComponent))
	{
		HitboxComponent->GetWorld()->SweepMultiByObjectType(
			Hits,
			Start,
			End,
			Rot,
			ObjectQueryParams,
			FCollisionShape::MakeBox(BoxComponent->GetScaledBoxExtent()),
			CollisionQueryParams
		);
	}
	return Hits;
}

TArray<FHitResult> FPerformAttackTrace::operator()(const FCapsuleAttackTrace& Capsule) const
{
	TArray<FHitResult> Hits;
	if (const UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(HitboxComponent))
	{
		HitboxComponent->GetWorld()->SweepMultiByObjectType(
			Hits,
			Start,
			End,
			Rot,
			ObjectQueryParams,
			FCollisionShape::MakeCapsule(CapsuleComponent->GetScaledCapsuleRadius(),
				CapsuleComponent->GetScaledCapsuleHalfHeight()),
			CollisionQueryParams
		);
	}
	return Hits;
}

FDrawDebugAttackTrace::FDrawDebugAttackTrace(UPrimitiveComponent* HitboxComponent,
	const FVector& Center, const FQuat& Rotation,
	const FColor& Color, bool bPersistentLines, float LifeTime)
	: HitboxComponent(HitboxComponent),
	  Center(Center), Rotation(Rotation),
	  Color(Color), bPersistentLines(bPersistentLines), LifeTime(LifeTime)
{
}

void FDrawDebugAttackTrace::operator()(const FEmptyAttackTrace& Empty) const
{
}

void FDrawDebugAttackTrace::operator()(const FBoxAttackTrace& Box) const
{
	if (const UBoxComponent* BoxComponent = Cast<UBoxComponent>(HitboxComponent))
	{
		DrawDebugBox(
			BoxComponent->GetWorld(),
			Center,
			BoxComponent->GetScaledBoxExtent(),
			Rotation,
			Color,
			bPersistentLines,
			LifeTime
		);
	}
}

void FDrawDebugAttackTrace::operator()(const FCapsuleAttackTrace& Capsule) const
{
	if (const UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(HitboxComponent))
	{
		DrawDebugCapsule(
			CapsuleComponent->GetWorld(),
			Center,
			CapsuleComponent->GetScaledCapsuleHalfHeight(),
			CapsuleComponent->GetScaledCapsuleRadius(),
			Rotation,
			Color,
			bPersistentLines,
			LifeTime
		);
	}
}
