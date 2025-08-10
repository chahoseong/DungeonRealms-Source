#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/DungeonRealmsAbilitySet.h"
#include "GameFramework/Character.h"
#include "DungeonRealmsCharacter.generated.h"

class UAttributeSet;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADungeonRealmsCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	template <typename T>
	T* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}

	UAttributeSet* GetAttributeSet() const;

	template <typename T>
	T* GetAttributeSet() const
	{
		return Cast<T>(GetAttributeSet());
	}

	AActor* GetAttachedActorFromSocket(FName SocketName) const;

protected:
	void InitializeAbilitySets();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TArray<TSoftObjectPtr<UDungeonRealmsAbilitySet>> AbilitySets;

private:
	FDungeonRealmsAbilitySetGrantedHandles GrantedHandles;
};
