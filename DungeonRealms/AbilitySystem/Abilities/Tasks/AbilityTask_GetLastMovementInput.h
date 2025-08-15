#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_GetLastMovementInput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLastMovementInputDelegete, FVector, LastMovementInput);

UCLASS()
class DUNGEONREALMS_API UAbilityTask_GetLastMovementInput : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="TRUE"))
	static UAbilityTask_GetLastMovementInput* GetLastMovementInput(UGameplayAbility* OwningAbility);

public:
	UAbilityTask_GetLastMovementInput(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
	UPROPERTY(BlueprintAssignable)
	FLastMovementInputDelegete OnLastMovementInput;

private:
	void SendLastMovementInput() const;
	
	UFUNCTION()
	void OnLastMovementInputReplicated();
	
private:
	EAbilityGenericReplicatedEvent::Type ReplicatedEventType = EAbilityGenericReplicatedEvent::GameCustom1;
	FDelegateHandle ReplicatedEventDelegate;
};
