#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "DungeonRealmsEnemyController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsEnemyController : public ADetourCrowdAIController
{
	GENERATED_BODY()

public:
	ADungeonRealmsEnemyController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin IGenericTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~End IGenericTeamAgentInterface interface
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnAIPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	void InitializeCrowdFollowingComponent() const;
	
protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Detour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Detour Crowd Avoidance Config", meta=(EditCondition="bEnableDetourCrowdAvoidance", UIMin="1", UIMax="4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Detour Crowd Avoidance Config", meta=(EditCondition="bEnableDetourCrowdAvoidance"))
	int32 AvoidanceGroup = 1;
	
	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Detour Crowd Avoidance Config", meta=(EditCondition="bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Blackboard")
	FName TargetActorKey;
};
