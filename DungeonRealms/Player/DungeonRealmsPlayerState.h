#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "DungeonRealmsPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADungeonRealmsPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
