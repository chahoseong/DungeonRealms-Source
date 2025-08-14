#include "AI/BehaviorTree/Tasks/BTTask_RotateToFaceTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("Rotate to face target actor");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
	
	INIT_TASK_NODE_NOTIFY_FLAGS();

	TargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetToFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardAsset = GetBlackboardAsset())
	{
		TargetToFaceKey.ResolveSelectedKey(*BlackboardAsset);
	}
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemory);
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString Key = TargetToFaceKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Smoothly rotates to face %s Key until the angle precision %s is reached"),
		*Key, *FString::SanitizeFloat(AnglePrecision));
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(Memory);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	Memory->OwningPawn = OwningPawn;

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetToFaceKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);
	Memory->TargetActor = TargetActor;

	if (!Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	if (HasReachedAnglePrecision(OwningPawn, TargetActor))
	{
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::InProgress;
}



void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);

	if (!Memory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (HasReachedAnglePrecision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			Memory->OwningPawn->GetActorLocation(),
			Memory->TargetActor->GetActorLocation()
		);
		const FRotator RotationStep = FMath::RInterpTo(
			Memory->OwningPawn->GetActorRotation(),
			TargetRotation,
			DeltaSeconds,
			RotationInterpSpeed
		);
		Memory->OwningPawn->SetActorRotation(RotationStep);
	}
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(const APawn* QueryPawn, const AActor* TargetActor) const
{
	const FVector QueryPawnForward = QueryPawn->GetActorForwardVector();
	const FVector QueryPawnToTargetActorDirection = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal2D();

	const float DotResult = FVector::DotProduct(QueryPawnForward, QueryPawnToTargetActorDirection);
	const float Angle = UKismetMathLibrary::DegAcos(DotResult);
	
	return Angle <= AnglePrecision;
}
