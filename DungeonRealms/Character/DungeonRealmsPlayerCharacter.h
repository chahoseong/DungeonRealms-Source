#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsCharacter.h"
#include "DungeonRealmsPlayerCharacter.generated.h"

class UDungeonRealmsEquipmentDefinition;
struct FInputActionValue;
class UDungeonRealmsEquipmentManagerComponent;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsPlayerCharacter : public ADungeonRealmsCharacter
{
	GENERATED_BODY()

public:
	ADungeonRealmsPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin APawn interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	//~End APawn interface
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

private:
	void InitializeAbilitySystem();
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDungeonRealmsEquipmentManagerComponent> EquipmentManagerComponent;

	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<TSubclassOf<UDungeonRealmsEquipmentDefinition>> StartupEquipments;
};
