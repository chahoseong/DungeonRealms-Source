#pragma once

#include "CoreMinimal.h"

#include "DungeonRealmsCombatSystemComponent.generated.h"

class UDungeonRealmsAttackTracer;

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

	UFUNCTION(BlueprintPure)
	bool CanDefendAgainst(const AActor* Attacker, float DefenseDegrees) const;
	
	bool IsGuarding() const;

private:
	void ApplyHitEvents(const TArray<FHitResult>& Hits) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<UDungeonRealmsAttackTracer> AttackTracerClass;

	UPROPERTY(Transient)
	TObjectPtr<UDungeonRealmsAttackTracer> ActiveAttackTracer; 
};
