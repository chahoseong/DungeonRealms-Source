#pragma once

#include "UObject/Object.h"
#include "DungeonRealmsEquipmentDefinition.h"
#include "DungeonRealmsEquipmentInstance.generated.h"

class UDungeonRealmsEquipmentAction;
struct FGameplayAbilitySpecHandle;

UCLASS(Blueprintable, BlueprintType)
class DUNGEONREALMS_API UDungeonRealmsEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	UDungeonRealmsEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin UObject interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End UObject interface
	
	TSubclassOf<UDungeonRealmsEquipmentDefinition> GetEquipmentDefinition() const;
	
	void AddEquipmentActor(AActor* EquipmentActor);
	void DestroyEquipmentActors();

	void AddGrantedAbilitySpecHandle(const FGameplayAbilitySpecHandle& SpecHandle);
	void ClearGrantedAbilities();
	

	APawn* GetPawn() const;
	bool HasAuthority() const;
	bool IsLocallyControlled() const;

	virtual void OnEquipped();
	virtual void OnUnequipped();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="DungeonRealms|Equipment", DisplayName="OnEquipped")
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category="DungeonRealms|Equipment", DisplayName="OnUnequipped")
	void K2_OnUnequipped();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<TObjectPtr<UDungeonRealmsEquipmentAction>> EquipmentActions;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TSubclassOf<UDungeonRealmsEquipmentDefinition> EquipmentDefinition;
	
	UPROPERTY(Transient, Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;

	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
