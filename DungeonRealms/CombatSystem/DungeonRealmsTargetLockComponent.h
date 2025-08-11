#pragma once

#include "CoreMinimal.h"

#include "DungeonRealmsTargetLockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetLocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetLost);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEONREALMS_API UDungeonRealmsTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDungeonRealmsTargetLockComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* InTarget);

	UFUNCTION(BlueprintPure)
	AActor* GetTarget() const;

	UPROPERTY(BlueprintAssignable)
	FOnTargetLocked OnTargetLocked;
	
	UPROPERTY(BlueprintAssignable)
	FOnTargetLost OnTargetLost;
	
protected:
	void UpdateOwnerRotation(float DeltaTime);
	bool IsFacingTarget(float Tolerance = 0.95f) const;
	
	bool IsTargetDead() const;
	bool IsOwnerDead() const;
	bool IsActorDead(AActor* Actor) const;
	
	AController* GetOwningController() const;

private:
	UFUNCTION()
	void OnRep_CurrentLockedTarget(AActor* LastLockedTarget);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentLockedTarget)
	TObjectPtr<AActor> CurrentLockedTarget;

	UPROPERTY(EditDefaultsOnly)
	float ViewPitchOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed = 5.0f;

private:
	bool bCachedOrientRotationToMovement;
	bool bShouldActorRotateTowardsTarget = false;
	bool bTargetLost = true;
};
