#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsGameplayAbility.h"
#include "DungeonRealmsGameplayAbility_LockOn.generated.h"

class UDungeonRealmsTargetLockComponent;
class UInputMappingContext;

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsGameplayAbility_LockOn : public UDungeonRealmsGameplayAbility
{
	GENERATED_BODY()

	public:
	UDungeonRealmsGameplayAbility_LockOn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& DirectionTag);

	UFUNCTION(Server, Reliable)
	void ServerSwitchTarget(const FGameplayTag& DirectionTag);
	
private:
	void TryLockOnTarget();
	void CancelTargetLock();

	void AddInputMapping() const;
	void RemoveInputMapping() const;
	
	TArray<AActor*> FindAvailableActorsToLock() const;
	AActor* GetNearestTargetFromActors(const FVector& Origin, const TArray<AActor*>& ActorsToCheck) const;
	void DivideAvailableActorsOnDirection(TArray<AActor*>& ActorsOnLeft, TArray<AActor*>& ActorsOnRight) const;

	void HandleTargetSwitching(const FGameplayTag& DirectionTag);
	
	UFUNCTION()
	void OnTargetLost();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	int32 InputPriority = 1;
	
	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	float TraceMaxDistance = 5000.0f;

	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	FVector TraceBoxExtend = FVector(2500.0f, 2500.0f, 150.0f);

	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceQueryChannel;

	UPROPERTY(EditDefaultsOnly, Category="Target Lock")
	bool bShowTraceDebug = false;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> AvailableActorsToLock;

private:
	TWeakObjectPtr<UDungeonRealmsTargetLockComponent> TargetLockComponent;
	bool bShouldActorRotateTowardsTarget = false;
};
