#include "CombatSystem/DungeonRealmsCombatStatics.h"
#include "Physics/DungeonRealmsCollisionChannels.h"

bool UDungeonRealmsCombatStatics::HasObstacleBetween(const AActor* SourceActor, const AActor* TargetActor)
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(SourceActor);
	CollisionQueryParams.AddIgnoredActor(TargetActor);
	CollisionQueryParams.bTraceComplex = false;

	FHitResult ObstacleHit;
	return SourceActor->GetWorld()->LineTraceSingleByChannel(
		ObstacleHit,
		SourceActor->GetActorLocation(),
		TargetActor->GetActorLocation(),
		DungeonRealms_TraceChannel_Obstacle,
		CollisionQueryParams
	);
}
