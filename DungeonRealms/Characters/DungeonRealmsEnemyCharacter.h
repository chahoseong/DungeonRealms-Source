#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsCharacter.h"
#include "UI/Controllers/DungeonRealmsOverlayController.h"
#include "DungeonRealmsEnemyCharacter.generated.h"

class UBehaviorTree;
struct FDungeonRealmsAttributeDefinition;
class UDungeonRealmsCombatSystemComponent;

USTRUCT(BlueprintType)
struct FEnemyWeaponToSpawn
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> WeaponActorClass;

	UPROPERTY(EditAnywhere)
	FName AttachSocketName;

	UPROPERTY(EditAnywhere)
	FTransform AttachTransform;
};

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsEnemyCharacter : public ADungeonRealmsCharacter
{
	GENERATED_BODY()

public:
	ADungeonRealmsEnemyCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual AActor* GetAttachedActorFromSocket(FName SocketName) const override;
	
	//~Begin APawn interface
	virtual void PossessedBy(AController* NewController) override;
	//~End APawn interface
	
	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature OnMaxHealthChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:
	void InitializeRetargetMeshComponent();
	void InitializeAbilitySystem();
	void InitializeAttributes();
	void BindAttributeChanges() const;
	void BroadcastInitialAttributes() const;
	
	void InitializeWeapons();
	void DestroySpawnedWeapons();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDungeonRealmsCombatSystemComponent> CombatSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms")
	int32 EnemyId;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Attributes", meta=(RowType="DungeonRealmsAttributeDefinition"))
	TObjectPtr<UDataTable> EnemyAttributesTable;
	
	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Combat")
	TArray<FEnemyWeaponToSpawn> WeaponActorsToSpawn;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> RetargetMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Combat")
	bool bUseRetargetMesh = false;

private:
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedWeapons;
};
