#pragma once

#include "CoreMinimal.h"

struct FEmptyAttackTrace { };
struct FBoxAttackTrace { };
struct FCapsuleAttackTrace { };

using FAttackTraceShape =
	TVariant<FEmptyAttackTrace, FBoxAttackTrace, FCapsuleAttackTrace>;

template <typename T>
concept IsAttackTraceShape =
	std::is_same_v<T, FEmptyAttackTrace> ||
	std::is_same_v<T, FBoxAttackTrace> ||
	std::is_same_v<T, FCapsuleAttackTrace>;

template <typename T>
FAttackTraceShape CreateAttackTraceShape() requires IsAttackTraceShape<T>
{
	FAttackTraceShape Shape;
	Shape.Emplace<T>();
	return Shape;
}

class FPerformAttackTrace
{
public:
	FPerformAttackTrace(UPrimitiveComponent* HitboxComponent,
		const FVector& Start,
		const FVector& End,
		const FQuat& Rot,
		const FCollisionObjectQueryParams& ObjectQueryParams,
		const FCollisionQueryParams& CollisionQueryParams
	);

	TArray<FHitResult> operator()(const FEmptyAttackTrace& Empty) const;
	TArray<FHitResult> operator()(const FBoxAttackTrace& Box) const;
	TArray<FHitResult> operator()(const FCapsuleAttackTrace& Capsule) const;

private:
	UPrimitiveComponent* HitboxComponent;
	FVector Start;
	FVector End;
	FQuat Rot;
	FCollisionObjectQueryParams ObjectQueryParams;
	FCollisionQueryParams CollisionQueryParams;
};

class FDrawDebugAttackTrace
{
public:
	FDrawDebugAttackTrace(UPrimitiveComponent* HitboxComponent,
		const FVector& Center,
		const FQuat& Rotation,
		const FColor& Color,
		bool bPersistentLines = false,
		float LifeTime = -1.0f);

	void operator()(const FEmptyAttackTrace& Empty) const;
	void operator()(const FBoxAttackTrace& Box) const;
	void operator()(const FCapsuleAttackTrace& Capsule) const;

private:
	UPrimitiveComponent* HitboxComponent;
	FVector Center;
	FQuat Rotation;
	FColor Color;
	bool bPersistentLines;
	float LifeTime;
};

