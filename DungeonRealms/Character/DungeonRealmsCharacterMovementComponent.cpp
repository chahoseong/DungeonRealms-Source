#include "Character/DungeonRealmsCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

namespace DungeonRealmsCharacter
{
	static float GroundTraceDistance = 10000.0f;
}

UDungeonRealmsCharacterMovementComponent::UDungeonRealmsCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

const FDungeonRealmsCharacterGroundInfo& UDungeonRealmsCharacterMovementComponent::GetGroundInfo() const
{
	if (!IsValid(CharacterOwner) || GFrameCounter == CachedGroundInfo.LastUpdateFrame)
	{
		return CachedGroundInfo;
	}
	
	if (MovementMode == MOVE_Walking)
	{
		CachedGroundInfo.GroundHit = CurrentFloor.HitResult;
		CachedGroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* Capsule = CharacterOwner->GetCapsuleComponent();
		check(Capsule);

		const float CapsuleHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = IsValid(UpdatedComponent) ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn;
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, TraceStart.Z - DungeonRealmsCharacter::GroundTraceDistance - CapsuleHalfHeight);

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(DungeonRealmsCharacterMovementComponent_GetGroundInfo), false, CharacterOwner);
		FCollisionResponseParams ResponseParams;
		InitCollisionParams(QueryParams, ResponseParams);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			CollisionChannel,
			QueryParams,
			ResponseParams
		);

		CachedGroundInfo.GroundHit = HitResult;
		CachedGroundInfo.GroundDistance = DungeonRealmsCharacter::GroundTraceDistance;

		if (MovementMode == MOVE_NavWalking)
		{
			CachedGroundInfo.GroundDistance = 0.0f;
		}
		else if (HitResult.bBlockingHit)
		{
			CachedGroundInfo.GroundDistance = FMath::Max(HitResult.Distance - CapsuleHalfHeight, 0.0f);
		}
	}

	CachedGroundInfo.LastUpdateFrame = GFrameCounter;

	return CachedGroundInfo;
}
