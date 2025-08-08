#include "AbilitySystem/DungeonRealmsAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "DungeonRealmsLogChannels.h"

void FDungeonRealmsAbilitySetGrantedHandles::AddGameplayAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayAbilitySpecHandles.Add(Handle);
	}
}

void FDungeonRealmsAbilitySetGrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FDungeonRealmsAbilitySetGrantedHandles::TakeFromAbilitySystem(UAbilitySystemComponent* TargetAbilitySystem)
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
	FDungeonRealmsAbilitySetGrantedHandles& OutHandles) const
{
	GiveToAbilitySystem(TargetAbilitySystem, nullptr, OutHandles);
}

void UDungeonRealmsAbilitySet::GiveToAbilitySystem(
	UAbilitySystemComponent* TargetAbilitySystem,
	UObject* SourceObject,
	FDungeonRealmsAbilitySetGrantedHandles& OutHandles) const
{
	check(TargetAbilitySystem);
	
	if (!TargetAbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
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
		const FGameplayAbilitySpecHandle Handle = TargetAbilitySystem->GiveAbility(AbilitySpec);
		OutHandles.AddGameplayAbilitySpecHandle(Handle);
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
		const FActiveGameplayEffectHandle Handle = TargetAbilitySystem->ApplyGameplayEffectToSelf(EffectDefault, EffectToApply.Level, TargetAbilitySystem->MakeEffectContext());
		OutHandles.AddGameplayEffectHandle(Handle);
	}
}
