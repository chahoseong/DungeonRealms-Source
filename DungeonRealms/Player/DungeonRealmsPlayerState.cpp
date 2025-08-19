#include "Player/DungeonRealmsPlayerState.h"
#include "AbilitySystem/DungeonRealmsAbilitySystemComponent.h"
#include "AbilitySystem/DungeonRealmsAttributeSet.h"
#include "Net/UnrealNetwork.h"

ADungeonRealmsPlayerState::ADungeonRealmsPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UDungeonRealmsAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UDungeonRealmsAttributeSet>(TEXT("AttributeSet"));

	SetNetUpdateFrequency(100.0f);
}

void ADungeonRealmsPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerIndex);
}

void ADungeonRealmsPlayerState::SetPlayerIndex(int32 NewPlayerIndex)
{
	PlayerIndex = NewPlayerIndex;
}

int32 ADungeonRealmsPlayerState::GetPlayerIndex() const
{
	return PlayerIndex;
}

UAbilitySystemComponent* ADungeonRealmsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ADungeonRealmsPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
