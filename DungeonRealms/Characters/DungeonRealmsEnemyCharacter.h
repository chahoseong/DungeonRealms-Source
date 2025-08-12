#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsCharacter.h"
#include "DungeonRealmsEnemyCharacter.generated.h"

struct FDungeonRealmsAttributeDefinition;
class UDungeonRealmsCombatSystemComponent;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsEnemyCharacter : public ADungeonRealmsCharacter
{
	GENERATED_BODY()

public:
	ADungeonRealmsEnemyCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

private:
	void InitializeAttributes();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDungeonRealmsCombatSystemComponent> CombatSystemComponent;

	UPROPERTY(EditDefaultsOnly)
	int32 EnemyId;

	UPROPERTY(EditDefaultsOnly, Category="Attributes", meta=(RowType="DungeonRealmsAttributeDefinition"))
	TObjectPtr<UDataTable> EnemyAttributesTable;
	
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
};
