#include "AbilitySystem/DungeonRealmsAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "DungeonRealmsLogChannels.h"

void FDungeonRealmsAbilitySetHandles::AddGameplayAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayAbilitySpecHandles.Add(Handle);
	}
}

void FDungeonRealmsAbilitySetHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FDungeonRealmsAbilitySetHandles::TakeFromAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);

	if (!AbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
	}

	// Clear granted gameplay abilities.
	for (const FGameplayAbilitySpecHandle& Handle : GameplayAbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->ClearAbility(Handle);
		}
	}
	GameplayAbilitySpecHandles.Reset();

	// Remove applied gameplay effects.
	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->RemoveActiveGameplayEffect(Handle);
		}
	}
	GameplayEffectHandles.Reset();
}

UDungeonRealmsAbilitySet::UDungeonRealmsAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FDungeonRealmsAbilitySetHandles UDungeonRealmsAbilitySet::GiveToAbilitySystem(
	UAbilitySystemComponent* AbilitySystem, UObject* SourceObject) const
{
	check(AbilitySystem);

	FDungeonRealmsAbilitySetHandles Handles;

	if (!AbilitySystem->IsOwnerActorAuthoritative())
	{
		return Handles;
	}

	// Grant the gameplay abilities.
	for (int32 Index = 0; Index < GrantedGameplayAbilities.Num(); ++Index)
	{
		const FGameplayAbilityToGrant& AbilityToGrant = GrantedGameplayAbilities[Index];
		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogDungeonRealms, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), Index, *GetNameSafe(this));
			continue;
		}
		UGameplayAbility* AbilityDefault = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(AbilityDefault, AbilityToGrant.Level);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);
		const FGameplayAbilitySpecHandle Handle = AbilitySystem->GiveAbility(AbilitySpec);
		Handles.AddGameplayAbilitySpecHandle(Handle);
	}

	// Apply the gameplay effects.
	for (int32 Index = 0; Index < AppliedGameplayEffects.Num(); ++Index)
	{
		const FGameplayEffectToApply& EffectToApply = AppliedGameplayEffects[Index];
		if (!IsValid(EffectToApply.Effect))
		{
			UE_LOG(LogDungeonRealms, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), Index, *GetNameSafe(this));
			continue;
		}
		const UGameplayEffect* EffectDefault = EffectToApply.Effect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle Handle = AbilitySystem->ApplyGameplayEffectToSelf(EffectDefault, EffectToApply.Level, AbilitySystem->MakeEffectContext());
		Handles.AddGameplayEffectHandle(Handle);
	}

	return Handles;
}
