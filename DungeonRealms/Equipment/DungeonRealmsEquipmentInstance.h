#pragma once

#include "UObject/Object.h"
#include "Equipment/DungeonRealmsEquipmentDefinition.h"
#include "AbilitySystem/DungeonRealmsAbilitySet.h"
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
	FDungeonRealmsEquipmentId GetEquipmentId() const;
	const FGameplayTagContainer& GetSlotTags() const;
	const TArray<FEquipmentAttributeBonus>& GetAttributeBonuses() const;
	
	void AddEquipmentActor(AActor* EquipmentActor);
	void DestroyEquipmentActors();
	
	APawn* GetPawn() const;

	template <typename T>
	T* GetPawn() const requires(std::is_base_of_v<APawn, T>)
	{
		return Cast<T>(GetPawn());
	}
	
	bool HasAuthority() const;
	bool IsLocallyControlled() const;

	virtual void OnEquipped();
	virtual void OnUnequipped();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="DungeonRealms|Equipment", DisplayName="OnEquipped")
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category="DungeonRealms|Equipment", DisplayName="OnUnequipped")
	void K2_OnUnequipped();

private:
	void GiveAbilitySetsToOwner(const TArray<UDungeonRealmsAbilitySet*>& AbilitySets);
	void TakeAbilitySetsFromOwner();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<TObjectPtr<UDungeonRealmsEquipmentAction>> EquipmentActions;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TSubclassOf<UDungeonRealmsEquipmentDefinition> EquipmentDefinition;

private:
	UPROPERTY(Transient, Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
	
	FDungeonRealmsAbilitySet_GrantedHandles GrantedHandles;
};
