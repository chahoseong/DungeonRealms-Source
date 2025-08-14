// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

UCLASS()
class DUNGEONREALMS_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_OrientToTargetActor();

	//~Begin BTNode interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	//~End BTNode interface

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	AActor* GetTargetActor(const UBlackboardComponent* Blackboard) const;
	void LookAtTargetFromController(const AAIController* Controller, const AActor* TargetActor, float DeltaSeconds) const;

private:
	UPROPERTY(EditAnywhere, Category="Target")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category="Target")
	float RotationInterpSpeed = 5.0f;
};
