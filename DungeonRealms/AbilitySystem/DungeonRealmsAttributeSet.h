#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DungeonRealmsAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UDungeonRealmsAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, Strength)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, Intelligence)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, Dexterity)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, Will)

	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, AttackDamage)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, AbilityPower)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, Armor)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, Resistance)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, MaxPoise)
	
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, Poise)
	
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, IncomingDamage)
	ATTRIBUTE_ACCESSORS(UDungeonRealmsAttributeSet, IncomingImpact)

private:
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;

	UFUNCTION()
	void OnRep_Will(const FGameplayAttributeData& OldWill) const;

	UFUNCTION()
	void OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage) const;

	UFUNCTION()
	void OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_Resistance(const FGameplayAttributeData& OldResistance) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_MaxPoise(const FGameplayAttributeData& OldMaxPoise) const;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_Poise(const FGameplayAttributeData& OldPoise) const;
	
protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Primary Attributes")
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, Category="Primary Attributes")
	FGameplayAttributeData Intelligence;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Dexterity, Category="Primary Attributes")
	FGameplayAttributeData Dexterity;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Will, Category="Primary Attributes")
	FGameplayAttributeData Will;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackDamage, Category="Secondary Attributes")
	FGameplayAttributeData AttackDamage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AbilityPower, Category="Secondary Attributes")
	FGameplayAttributeData AbilityPower;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Secondary Attributes")
	FGameplayAttributeData Armor;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resistance, Category="Secondary Attributes")
	FGameplayAttributeData Resistance;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Secondary Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Secondary Attributes")
	FGameplayAttributeData MaxMana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxPoise, Category="Secondary Attributes")
	FGameplayAttributeData MaxPoise;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Poise, Category="Vital Attributes")
	FGameplayAttributeData Poise;
	
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;

	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingImpact;
};
