#include "AI/BehaviorTree/Tasks/BTTask_TryActivateAbilityByClass.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

UBTTask_TryActivateAbilityByClass::UBTTask_TryActivateAbilityByClass()
{
	NodeName = TEXT("Try Activate Ability By Class");
	bNotifyTick = false;
	bNotifyTaskFinished = false;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

FString UBTTask_TryActivateAbilityByClass::GetStaticDescription() const
{
	const FString ClassName = AbilityToActivate ? AbilityToActivate->GetName() : TEXT("");
	return  FString::Printf(TEXT("Try activate ability by class: %s"), *ClassName);
}

EBTNodeResult::Type UBTTask_TryActivateAbilityByClass::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!AbilityToActivate)
	{
		return EBTNodeResult::Aborted;
	}

	const AController* Owner = OwnerComp.GetOwner<AController>();
	UAbilitySystemComponent* OwningAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner->GetPawn());
	
	if (!IsValid(OwningAbilitySystem))
	{
		return EBTNodeResult::Aborted;
	}

	const bool bActivated = OwningAbilitySystem->TryActivateAbilityByClass(AbilityToActivate, false);
	return bActivated ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
