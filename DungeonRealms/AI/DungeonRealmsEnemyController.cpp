#include "AI/DungeonRealmsEnemyController.h"

#include "DungeonRealmsLogChannels.h"
#include "CombatSystem/DungeonRealmsCombatStatics.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Team/DungeonRealmsTeam.h"

ADungeonRealmsEnemyController::ADungeonRealmsEnemyController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnAIPerceptionUpdated);
}

void ADungeonRealmsEnemyController::BeginPlay()
{
	Super::BeginPlay();
	InitializeCrowdFollowingComponent();
}

void ADungeonRealmsEnemyController::InitializeCrowdFollowingComponent() const
{
	if (UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdFollowingComponent->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:
			CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
			break;
		case 2:
			CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
			break;
		case 3:
			CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
			break;
		case 4:
			CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			break;
		}
		CrowdFollowingComponent->SetAvoidanceGroup(AvoidanceGroup);
		CrowdFollowingComponent->SetGroupsToAvoid(AvoidanceGroup);
		CrowdFollowingComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void ADungeonRealmsEnemyController::OnAIPerceptionUpdated_Implementation(AActor* Actor, FAIStimulus Stimulus)
{

}

void ADungeonRealmsEnemyController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	UE_LOG(LogDungeonRealms, Error, TEXT("You can't set the team id (%s); This controller's team id have to enemy."), *GetPathNameSafe(this));
}

FGenericTeamId ADungeonRealmsEnemyController::GetGenericTeamId() const
{
	return ToGenericTeamId(EDungeonRealmsTeam::Enemy);
}

ETeamAttitude::Type ADungeonRealmsEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	return UDungeonRealmsCombatStatics::GetTeamAttitudeTowards(GetPawn(), &Other);
}
