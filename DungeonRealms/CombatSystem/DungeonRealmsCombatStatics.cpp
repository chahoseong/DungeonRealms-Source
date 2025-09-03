#include "CombatSystem/DungeonRealmsCombatStatics.h"

#include "DungeonRealmsCombatSystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Physics/DungeonRealmsCollisionChannels.h"
#include "Team/DungeonRealmsTeam.h"

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

bool UDungeonRealmsCombatStatics::HasHostileAttitude(const AActor* QueryActor, const AActor* TargetActor)
{
	return GetTeamAttitudeTowards(QueryActor, TargetActor) == ETeamAttitude::Hostile;
}

ETeamAttitude::Type UDungeonRealmsCombatStatics::GetTeamAttitudeTowards(const AActor* QueryActor,
	const AActor* TargetActor)
{
	const IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryActor);
	const IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetActor);

	if (QueryTeamAgent && TargetTeamAgent)
	{
		const bool bQueryActorHasTeam = QueryTeamAgent->GetGenericTeamId() != ToGenericTeamId(EDungeonRealmsTeam::NoTeam);
		const bool bTargetActorHasTeam = TargetTeamAgent->GetGenericTeamId() != ToGenericTeamId(EDungeonRealmsTeam::NoTeam);
		if (bQueryActorHasTeam && bTargetActorHasTeam)
		{
			const EDungeonRealmsTeam QueryTeam = ToDungeonRealmsTeam(QueryTeamAgent->GetGenericTeamId());
			const EDungeonRealmsTeam TargetTeam = ToDungeonRealmsTeam(TargetTeamAgent->GetGenericTeamId());
			return QueryTeam != TargetTeam
				       ? ETeamAttitude::Hostile
				       : ETeamAttitude::Friendly;
		}
	}
	
	return ETeamAttitude::Neutral;
}

void UDungeonRealmsCombatStatics::ApplyDamageEffectToTarget(const AActor* TargetActor, const FDamageSpec& DamageSpec)
{
	if (UDungeonRealmsCombatSystemComponent* TargetCombatSystem = UDungeonRealmsCombatSystemComponent::FindCombatSystemComponent(TargetActor))
	{
		TargetCombatSystem->ApplyDamageEffect(DamageSpec);
	}
}
