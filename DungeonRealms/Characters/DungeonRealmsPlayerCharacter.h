#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsCharacter.h"
#include "CombatSystem/DungeonRealmsCombatSystemInterface.h"
#include "DungeonRealmsPlayerCharacter.generated.h"

class UDungeonRealmsTargetLockComponent;
class UDungeonRealmsCombatSystemComponent;
class UDungeonRealmsEquipmentDefinition;
struct FInputActionValue;
class UDungeonRealmsEquipmentManagerComponent;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsPlayerCharacter : public ADungeonRealmsCharacter, public IDungeonRealmsCombatSystemInterface
{
	GENERATED_BODY()

public:
	ADungeonRealmsPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void Equip(TSubclassOf<UDungeonRealmsEquipmentDefinition> EquipmentDefinition);
	
	//~Begin APawn interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	//~End APawn interface

	UDungeonRealmsCombatSystemComponent* GetCombatSystemComponent() const;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

protected:
	UFUNCTION(Server, Reliable)
	void ServerEquip(TSubclassOf<UDungeonRealmsEquipmentDefinition> EquipmentDefinition);
	
private:
	void InitializeAbilitySystem();
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDungeonRealmsEquipmentManagerComponent> EquipmentManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDungeonRealmsCombatSystemComponent> CombatSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDungeonRealmsTargetLockComponent> TargetLockComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Equipment")
	TArray<TSubclassOf<UDungeonRealmsEquipmentDefinition>> StartupEquipments;

	UPROPERTY(EditDefaultsOnly, Category="DungeonRealms|Character")
	TArray<TObjectPtr<UMaterialInstance>> CharacterMaterials;
};
