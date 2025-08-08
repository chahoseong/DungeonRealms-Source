#include "Character/DungeonRealmsCharacter.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

ADungeonRealmsCharacter::ADungeonRealmsCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ADungeonRealmsCharacter::InitializeAbilitySets()
{
	TArray<FSoftObjectPath> Paths;
	
	for (TSoftObjectPtr<UDungeonRealmsAbilitySet> AbilitySet : AbilitySets)
	{
		if (AbilitySet.IsPending())
		{
			Paths.Add(AbilitySet.ToSoftObjectPath());
		}
		else
		{
			AbilitySet.Get()->GiveToAbilitySystem(
				AbilitySystemComponent,
				this,
				GrantedHandles
			);
		}
	}

	if (Paths.Num() > 0)
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		TWeakObjectPtr ThisCharacter = this;
		Streamable.RequestAsyncLoad(Paths, FStreamableDelegate::CreateLambda([ThisCharacter]()
		{
			if (ThisCharacter.IsValid())
			{
				ThisCharacter->InitializeAbilitySets();
			}
		}));
	}
}

UAbilitySystemComponent* ADungeonRealmsCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ADungeonRealmsCharacter::GetAttributeSet() const
{
	return AttributeSet;
}
