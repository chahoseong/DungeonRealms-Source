#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsCombatSystemComponent.generated.h"

class UGameplayEffect;
class UDungeonRealmsAttackTracer;

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
	TSubclassOf<UDungeonRealmsAttackTracer> AttackTracerClass;

	UPROPERTY(Transient)
	TObjectPtr<UDungeonRealmsAttackTracer> ActiveAttackTracer;

private:
	float DefenseDegrees = 0.0f;
	bool bIsGuarding = false;
};
