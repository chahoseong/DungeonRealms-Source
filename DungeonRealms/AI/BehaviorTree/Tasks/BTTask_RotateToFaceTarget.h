#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

struct FRotateToFaceTargetTaskMemory
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
	
	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}
};

UCLASS()
class DUNGEONREALMS_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RotateToFaceTarget();

	//~Begin UBTTaskNode interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	//~End UBTTaskNode interface

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool HasReachedAnglePrecision(const APawn* QueryPawn, const AActor* TargetActor) const;

private:
	UPROPERTY(EditAnywhere)
	float AnglePrecision = 10.0f;

	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetToFaceKey;
};
