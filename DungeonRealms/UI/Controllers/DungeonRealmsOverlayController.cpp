#include "UI/Controllers/DungeonRealmsOverlayController.h"
#include "AbilitySystem/DungeonRealmsAbilitySystemComponent.h"
#include "AbilitySystem/DungeonRealmsAttributeSet.h"
#include "Player/DungeonRealmsPlayerState.h"

void UDungeonRealmsOverlayController::Initialize(APlayerController* NewPlayerController)
{
	Super::Initialize(NewPlayerController);
	
	const ADungeonRealmsPlayerState* DungeonRealmsPlayerState = GetPlayerState<ADungeonRealmsPlayerState>();
	UDungeonRealmsAbilitySystemComponent* DungeonRealmsAbilitySystem =
		DungeonRealmsPlayerState->GetAbilitySystemComponent<UDungeonRealmsAbilitySystemComponent>();

	// Broadcast health changed
	DungeonRealmsAbilitySystem->GetGameplayAttributeValueChangeDelegate(UDungeonRealmsAttributeSet::GetHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});
	
	// Broadcast max health changed
	DungeonRealmsAbilitySystem->GetGameplayAttributeValueChangeDelegate(UDungeonRealmsAttributeSet::GetMaxHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});
	
	// Broadcast mana changed
	DungeonRealmsAbilitySystem->GetGameplayAttributeValueChangeDelegate(UDungeonRealmsAttributeSet::GetManaAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});
	
	// Broadcast max mana changed
	DungeonRealmsAbilitySystem->GetGameplayAttributeValueChangeDelegate(UDungeonRealmsAttributeSet::GetMaxManaAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});
}

void UDungeonRealmsOverlayController::BroadcastInitialValues() const
{
	if (const UDungeonRealmsAttributeSet* DungeonRealmsAttributeSet = GetAttributeSet())
	{
		OnHealthChanged.Broadcast(DungeonRealmsAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(DungeonRealmsAttributeSet->GetMaxHealth());
		OnManaChanged.Broadcast(DungeonRealmsAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(DungeonRealmsAttributeSet->GetMaxMana());
	}
}

UDungeonRealmsAttributeSet* UDungeonRealmsOverlayController::GetAttributeSet() const
{
	const ADungeonRealmsPlayerState* DungeonRealmsPlayerState = GetPlayerState<ADungeonRealmsPlayerState>();
	return DungeonRealmsPlayerState ?
		DungeonRealmsPlayerState->GetAttributeSet<UDungeonRealmsAttributeSet>() :
		nullptr;
}
