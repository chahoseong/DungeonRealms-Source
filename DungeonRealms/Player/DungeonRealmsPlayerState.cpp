#include "Player/DungeonRealmsPlayerState.h"
#include "AbilitySystem/DungeonRealmsAbilitySystemComponent.h"
#include "AbilitySystem/DungeonRealmsAttributeSet.h"

ADungeonRealmsPlayerState::ADungeonRealmsPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UDungeonRealmsAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UDungeonRealmsAttributeSet>(TEXT("AttributeSet"));

	SetNetUpdateFrequency(100.0f);
}

UAbilitySystemComponent* ADungeonRealmsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ADungeonRealmsPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
