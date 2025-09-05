#pragma once

#include "CoreMinimal.h"
#include "FDungeonRealmsAttackTracer.h"
#include "DungeonRealmsCombatSystemComponent.generated.h"

class IDungeonRealmsCombatContext;
struct FGameplayTag;
class UGameplayEffect;
class FDungeonRealmsAttackTracer;

UENUM(BlueprintType)
enum class EGuardResult : uint8
{
	Block,
	Parry,
	Failed,
};

USTRUCT(BlueprintType)
struct FHitEventData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* Instigator = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* TargetActor = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bBlocked = false;

	UPROPERTY(BlueprintReadOnly)
	bool bParried = false;
};

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
	bool bAttackBlocked = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bShouldKnockdown = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitsDelegate, const TArray<FHitEventData>&, HitEvents);

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
	void SphereAttackTrace(
		const FVector& Location,
		float Radius
	);

	UFUNCTION(BlueprintCallable)
	void CapsuleAttackTrace(
		const FVector& Location,
		const FRotator& Rotation,
		float HalfHeight,
		float Radius
	);

	UFUNCTION(BlueprintCallable)
	void SetDefensibleAngle(float Degrees);

	UFUNCTION(BlueprintCallable)
	void SetParryable(bool bEnabled);

	UFUNCTION(BlueprintPure)
	bool IsParryable() const;

	bool IsGuardStance() const;
	FHitEventData ResolveHitEvent(const AActor* Instigator, const FHitEventData& Payload) const;

	UFUNCTION(BlueprintCallable)
	void ApplyDamageEffect(const FDamageSpec& DamageSpec);

	UPROPERTY(BlueprintAssignable)
	FOnHitsDelegate OnAttackHits;

	UPROPERTY(BlueprintAssignable)
	FOnHitsDelegate OnGuardHits;

private:
	void ImmediateAttackTrace(
		const FVector& Location,
		const FRotator& Rotation,
		const FCollisionShape& Shape,
		const TFunction<void(bool)>& DebugDrawer
	) const;
	TArray<FHitResult> FilterToHostileTargets(const TArray<FHitResult>& Hits) const;
	void ProcessHitEvents(const TArray<FHitResult>& Hits) const;

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
	float DefensibleDegrees = 0.0f;
	bool bParryable = false;
};
