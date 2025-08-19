#pragma once

#include "CoreMinimal.h"
#include "FDungeonRealmsAttackTracer.h"
#include "DungeonRealmsCombatSystemComponent.generated.h"

class UGameplayEffect;
class FDungeonRealmsAttackTracer;

USTRUCT(BlueprintType)
struct FDamageSpec
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UObject> SourceObject;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Instigator;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> DamageCauser;

	UPROPERTY(BlueprintReadWrite)
	float AttackDamageCoefficient = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityPowerCoefficient = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float DamageImpact = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float KnockbackPower = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	bool bShouldKnockdown = false;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEONREALMS_API UDungeonRealmsCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="DungeonRealms|CombatSystem")
	static UDungeonRealmsCombatSystemComponent* FindCombatSystemComponent(const AActor* Actor);

public:
	UDungeonRealmsCombatSystemComponent();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void BeginAttackTrace(FName SocketName);

	UFUNCTION(BlueprintCallable)
	void PerformAttackTrace();

	UFUNCTION(BlueprintCallable)
	void EndAttackTrace();

	UFUNCTION(BlueprintCallable)
	void BeginGuard(float InDefenseDegrees);

	UFUNCTION(BlueprintCallable)
	void EndGuard();

	bool CanDefendAgainst(const UDungeonRealmsCombatSystemComponent* Attacker) const;
	
	UFUNCTION(BlueprintCallable)
	void ApplyDamageEffect(const FDamageSpec& DamageSpec);

private:
	TArray<FHitResult> FilterToHostileTargets(const TArray<FHitResult>& Hits) const;
	void ApplyHitEvents(const TArray<FHitResult>& Hits) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TArray<TEnumAsByte<EObjectTypeQuery>> AttackTraceObjectTypes;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	int32 AttackTraceSubsteps = 2;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	bool bDrawDebugAttackTrace = false;

private:
	FDungeonRealmsAttackTracer AttackTracer;
	TWeakObjectPtr<AActor> AttackTraceSourceActor;
	
	float DefenseDegrees = 0.0f;
	bool bIsGuarding = false;
};
