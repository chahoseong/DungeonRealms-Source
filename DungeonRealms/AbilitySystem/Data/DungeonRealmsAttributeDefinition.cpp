#include "AbilitySystem/Data/DungeonRealmsAttributeDefinition.h"

#include "DungeonRealmsGameplayTags.h"

TMap<FGameplayTag, float> FDungeonRealmsAttributeDefinition::GetPrimaryAttributeMagnitudes() const
{
	TMap<FGameplayTag, float> Magnitudes;
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_Strength, Strength);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_Intelligence, Intelligence);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_Dexterity, Dexterity);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_Will, Will);
	return Magnitudes;
}

TMap<FGameplayTag, float> FDungeonRealmsAttributeDefinition::GetSecondaryAttributeMagnitudes() const
{
	TMap<FGameplayTag, float> Magnitudes;
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_AttackDamage, AttackDamage);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_AbilityPower, AbilityPower);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_Armor, Armor);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_Resistance, Resistance);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_MaxHealth, MaxHealth);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_MaxStamina, MaxStamina);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_MaxMana, MaxMana);
	Magnitudes.Add(DungeonRealmsGameplayTags::SetByCaller_Attribute_MaxPoise, MaxPoise);
	return Magnitudes;
}
