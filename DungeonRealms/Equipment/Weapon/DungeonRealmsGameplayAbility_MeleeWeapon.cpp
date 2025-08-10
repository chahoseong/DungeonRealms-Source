#include "Equipment/Weapon/DungeonRealmsGameplayAbility_MeleeWeapon.h"
#include "AbilitySystemComponent.h"

UDungeonRealmsGameplayAbility_MeleeWeapon::UDungeonRealmsGameplayAbility_MeleeWeapon(
	const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDungeonRealmsGameplayAbility_MeleeWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bWasComboCommitted = false;
}

void UDungeonRealmsGameplayAbility_MeleeWeapon::CommitCombo()
{
	if (bWasComboCommitted)
	{
		return;
	}

	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (const FGameplayAbilitySpec* NextAttackSpec = AbilitySystemComponent->FindAbilitySpecFromClass(NextAttackClass))
	{
		if (HasAuthority(&CurrentActivationInfo))
		{
			AdvanceCombo(NextAttackSpec->Handle);
		}
		else
		{
			ServerCommitCombo(NextAttackSpec->Handle);
			AdvanceCombo(NextAttackSpec->Handle);
		}
	}
}

void UDungeonRealmsGameplayAbility_MeleeWeapon::ServerCommitCombo_Implementation(
	FGameplayAbilitySpecHandle PredicatedAbilitySpecHandle)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	
	if (IsComboWindowOpen() && IsCorrectPrediction(PredicatedAbilitySpecHandle))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	else
	{
		AbilitySystemComponent->CancelAbilityHandle(PredicatedAbilitySpecHandle);
	}
}

bool UDungeonRealmsGameplayAbility_MeleeWeapon::IsCorrectPrediction(
	FGameplayAbilitySpecHandle PredictedAbilitySpecHandle) const
{
	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(PredictedAbilitySpecHandle))
	{
 		return NextAttackClass == AbilitySpec->Ability->GetClass();
	}
	return false;
}

void UDungeonRealmsGameplayAbility_MeleeWeapon::AdvanceCombo(FGameplayAbilitySpecHandle AbilitySpecHandle)
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	AbilitySystem->TryActivateAbility(AbilitySpecHandle);
	bWasComboCommitted = true;
}

void UDungeonRealmsGameplayAbility_MeleeWeapon::OpenComboWindow()
{
	bComboWindowOpen = true;
}

void UDungeonRealmsGameplayAbility_MeleeWeapon::CloseComboWindow()
{
	bComboWindowOpen = false;
	ComboWindowCloseTime = GetWorld()->GetTimeSeconds();
}

bool UDungeonRealmsGameplayAbility_MeleeWeapon::IsComboWindowOpen() const
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		const float ComboCommitLatency = GetWorld()->GetTimeSeconds() - ComboWindowCloseTime;
		return bComboWindowOpen || ComboCommitLatency <= AllowedComboWindowTolerance;
	}
	else
	{
		return bComboWindowOpen;
	}
}
