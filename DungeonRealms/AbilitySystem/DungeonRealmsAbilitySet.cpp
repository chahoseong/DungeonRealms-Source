#include "AbilitySystem/DungeonRealmsAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "DungeonRealmsLogChannels.h"

void FDungeonRealmsAbilitySet_GrantedHandles::AddGameplayAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayAbilitySpecHandles.Add(Handle);
	}
}

void FDungeonRealmsAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FDungeonRealmsAbilitySet_GrantedHandles::TakeFromAbilitySystem(UAbilitySystemComponent* TargetAbilitySystem)
{
	check(TargetAbilitySystem);

	if (!TargetAbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
	}

	// Clear granted gameplay abilities.
	for (const FGameplayAbilitySpecHandle& Handle : GameplayAbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			TargetAbilitySystem->ClearAbility(Handle);
		}
	}
	GameplayAbilitySpecHandles.Reset();

	// Remove applied gameplay effects.
	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			TargetAbilitySystem->RemoveActiveGameplayEffect(Handle);
		}
	}
	GameplayEffectHandles.Reset();
}

UDungeonRealmsAbilitySet::UDungeonRealmsAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDungeonRealmsAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* TargetAbilitySystem,
	FDungeonRealmsAbilitySet_GrantedHandles& OutHandles) const
{
	GiveToAbilitySystem(TargetAbilitySystem, nullptr, OutHandles);
}

void UDungeonRealmsAbilitySet::GiveToAbilitySystem(
	UAbilitySystemComponent* TargetAbilitySystem,
	UObject* SourceObject,
	FDungeonRealmsAbilitySet_GrantedHandles& OutHandles) const
{
	check(TargetAbilitySystem);
	
	if (!TargetAbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
	}

	// Grant the active abilities.
	for (int32 Index = 0; Index < ActiveAbilities.Num(); ++Index)
	{
		const FDungeonRealmsAbilitySet_ActiveGameplayAbility& AbilityToGrant = ActiveAbilities[Index];
		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogDungeonRealms, Error, TEXT("ActiveAbilities[%d] on ability set [%s] is not valid."), Index, *GetNameSafe(this));
			continue;
		}
		UGameplayAbility* AbilityDefault = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(AbilityDefault, AbilityToGrant.Level);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);
		const FGameplayAbilitySpecHandle Handle = TargetAbilitySystem->GiveAbility(AbilitySpec);
		OutHandles.AddGameplayAbilitySpecHandle(Handle);
	}

	// Grant the reactive abilities.
	for (int32 Index = 0; Index < ReactiveAbilities.Num(); ++Index)
	{
		const FDungeonRealmsAbilitySet_ReactiveGameplayAbility& AbilityToGrant = ReactiveAbilities[Index];
		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogDungeonRealms, Error, TEXT("ReactiveAbilities[%d] on ability set [%s] is not valid."), Index, *GetNameSafe(this));
			continue;
		}
		UGameplayAbility* AbilityDefault = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(AbilityDefault, AbilityToGrant.Level);
		AbilitySpec.SourceObject = SourceObject;
		const FGameplayAbilitySpecHandle Handle = TargetAbilitySystem->GiveAbility(AbilitySpec);
		OutHandles.AddGameplayAbilitySpecHandle(Handle);
	}

	// Apply the gameplay effects.
	for (int32 Index = 0; Index < GameplayEffects.Num(); ++Index)
	{
		const FDungeonRealmsAbilitySet_GameplayEffect& EffectToApply = GameplayEffects[Index];
		if (!IsValid(EffectToApply.Effect))
		{
			UE_LOG(LogDungeonRealms, Error, TEXT("GameplayEffects[%d] on ability set [%s] is not valid"), Index, *GetNameSafe(this));
			continue;
		}
		const UGameplayEffect* EffectDefault = EffectToApply.Effect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle Handle = TargetAbilitySystem->ApplyGameplayEffectToSelf(EffectDefault, EffectToApply.Level, TargetAbilitySystem->MakeEffectContext());
		OutHandles.AddGameplayEffectHandle(Handle);
	}
}
