#include "DungeonRealmsGameplayTags.h"

namespace DungeonRealmsGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked")

	UE_DEFINE_GAMEPLAY_TAG(Equipment_Slot_MainHand, "Equipment.Slot.MainHand")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Slot_OffHand, "Equipment.Slot.OffHand")

	UE_DEFINE_GAMEPLAY_TAG(State_Guarding, "State.Guarding")
	UE_DEFINE_GAMEPLAY_TAG(State_Rolling, "State.Rolling")
	UE_DEFINE_GAMEPLAY_TAG(State_Dead, "State.Dead")
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Move, "Input.Action.Move")
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Look, "Input.Action.Look")
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_SwitchTarget, "Input.Action.SwitchTarget")

	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_Hit, "Event.Attack.Hit")
	UE_DEFINE_GAMEPLAY_TAG(Event_SwitchTarget_Left, "Event.SwitchTarget.Left")
	UE_DEFINE_GAMEPLAY_TAG(Event_SwitchTarget_Right, "Event.SwitchTarget.Right")
	UE_DEFINE_GAMEPLAY_TAG(Event_Health_Damaged, "Event.Health.Damaged")
	UE_DEFINE_GAMEPLAY_TAG(Event_Dead, "Event.Dead")

	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_Strength, "SetByCaller.Attribute.Strength")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_Intelligence, "SetByCaller.Attribute.Intelligence")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_Dexterity, "SetByCaller.Attribute.Dexterity")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_Will, "SetByCaller.Attribute.Will")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_AttackDamage, "SetByCaller.Attribute.AttackDamage")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_AbilityPower, "SetByCaller.Attribute.AbilityPower")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_Armor, "SetByCaller.Attribute.Armor")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_Resistance, "SetByCaller.Attribute.Resistance")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_MaxHealth, "SetByCaller.Attribute.MaxHealth")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Attribute_MaxMana, "SetByCaller.Attribute.MaxMana")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Damage_AttackDamage_Coefficient, "SetByCaller.Damage.AttackDamage.Coefficient")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Damage_AbilityPower_Coefficient, "SetByCaller.Damage.AbilityPower.Coefficient")
}
