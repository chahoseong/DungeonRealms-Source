#include "AI/BehaviorTree/Services/BTService_OrientToTargetActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	NodeName = TEXT("Native orient rotation to target actor");
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
	Interval = 0.0f;
	RandomDeviation = 0.0f;
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorKey), AActor::StaticClass());
}

void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		TargetActorKey.ResolveSelectedKey(*BlackboardData);
	}
}

FString UBTService_OrientToTargetActor::GetStaticDescription() const
{
	const FString KeyName = TargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Orient rotation to %s key %s"), *KeyName, *GetStaticServiceDescription());
}

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		LookAtTargetFromController(OwnerComp.GetAIOwner(), GetTargetActor(Blackboard), DeltaSeconds);
	}
}

AActor* UBTService_OrientToTargetActor::GetTargetActor(const UBlackboardComponent* Blackboard) const
{
	UObject* Object = Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName);
	return Cast<AActor>(Object);
}

void UBTService_OrientToTargetActor::LookAtTargetFromController(const AAIController* Controller, const AActor* TargetActor, float DeltaSeconds) const
{
	if (Controller && TargetActor)
	{
		if (APawn* OwningPawn = Controller->GetPawn())
		{
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
			const FRotator RotationStep = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRotation, DeltaSeconds, RotationInterpSpeed);
			OwningPawn->SetActorRotation(RotationStep);
		}
	}
}
