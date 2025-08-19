#include "AbilitySystem/DungeonRealmsAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DungeonRealmsGameplayEffectContext.h"
#include "DungeonRealmsGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UDungeonRealmsAttributeSet::UDungeonRealmsAttributeSet()
{
}

void UDungeonRealmsAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, Will, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, AttackDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, AbilityPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, Resistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, MaxPoise, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, PoiseRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDungeonRealmsAttributeSet, Poise, COND_None, REPNOTIFY_Always);
}

void UDungeonRealmsAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}

	if (Attribute == GetPoiseAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxPoise());
	}
}

void UDungeonRealmsAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetPoiseAttribute())
	{
		SetPoise(FMath::Clamp(GetPoise(), 0.0f, GetMaxPoise()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float IncomingDamageMagnitude = GetIncomingDamage();
		SetIncomingDamage(0.0f);

		if (IncomingDamageMagnitude > 0.0f)
		{
			const float NewHealth = GetHealth() - IncomingDamageMagnitude;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			FDungeonRealmsGameplayEffectContext* ExtraEffectContext = FDungeonRealmsGameplayEffectContext::ExtraEffectContext(Data.EffectSpec.GetContext());
			const float NewPoise = GetPoise() - ExtraEffectContext->GetDamageImpact();
			SetPoise(FMath::Clamp(NewPoise, 0.0f, GetMaxPoise()));

			const FGameplayTag EventTag = NewHealth > 0.0f
										   ? DungeonRealmsGameplayTags::Event_Damaged
										   : DungeonRealmsGameplayTags::Event_Dead;
			FGameplayEventData EventData;
			EventData.ContextHandle = Data.EffectSpec.GetContext();
			EventData.Instigator = Data.EffectSpec.GetContext().GetEffectCauser();
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				GetOwningActor(),
				EventTag,
				EventData
			);
			
			UE_LOG(LogTemp, Warning, TEXT("Incoming Damage: %f, Current Health: %f"), IncomingDamageMagnitude, GetHealth());
		}
	}
}

void UDungeonRealmsAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, Strength, OldStrength);
}

void UDungeonRealmsAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, Intelligence, OldIntelligence);
}

void UDungeonRealmsAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, Dexterity, OldDexterity);
}

void UDungeonRealmsAttributeSet::OnRep_Will(const FGameplayAttributeData& OldWill) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, Will, OldWill);
}

void UDungeonRealmsAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, AttackDamage, OldAttackDamage);
}

void UDungeonRealmsAttributeSet::OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, AbilityPower, OldAbilityPower);
}

void UDungeonRealmsAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, Armor, OldArmor);
}

void UDungeonRealmsAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, Resistance, OldResistance);
}

void UDungeonRealmsAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, MaxHealth, OldMaxHealth);
}

void UDungeonRealmsAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, MaxMana, OldMaxMana);
}

void UDungeonRealmsAttributeSet::OnRep_MaxPoise(const FGameplayAttributeData& OldMaxPoise) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, MaxPoise, OldMaxPoise);
}

void UDungeonRealmsAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UDungeonRealmsAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UDungeonRealmsAttributeSet::OnRep_PoiseRegeneration(const FGameplayAttributeData& OldPoiseRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, PoiseRegeneration, PoiseRegeneration);
}

void UDungeonRealmsAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, Health, OldHealth);
}

void UDungeonRealmsAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, Mana, OldMana);
}

void UDungeonRealmsAttributeSet::OnRep_Poise(const FGameplayAttributeData& OldPoise) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDungeonRealmsAttributeSet, Poise, OldPoise);
}
