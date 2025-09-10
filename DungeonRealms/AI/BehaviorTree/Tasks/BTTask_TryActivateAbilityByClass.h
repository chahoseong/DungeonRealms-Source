#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryActivateAbilityByClass.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class DUNGEONREALMS_API UBTTask_TryActivateAbilityByClass : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TryActivateAbilityByClass();

	//~Begin UBTTaskNode interface
	virtual FString GetStaticDescription() const override;
	//~End UBTTaskNode interface
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityToActivate;
};
