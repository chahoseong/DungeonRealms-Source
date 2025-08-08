#include "Equipment/Weapon/DungeonRealmsWeaponInstance.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DungeonRealmsWeaponDefinition.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"

void UDungeonRealmsWeaponInstance::OnEquipped()
{
	Super::OnEquipped();
	
 	const UDungeonRealmsWeaponDefinition* WeaponDefinition = GetDefault<UDungeonRealmsWeaponDefinition>(GetEquipmentDefinition());

	if (IsLocallyControlled() && IsValid(WeaponDefinition->InputMappingContext))
	{
		AddInputMappingContext(WeaponDefinition->InputMappingContext, WeaponDefinition->InputPriority);
	}
	if (IsValid(WeaponDefinition->AnimClassLayers))
	{
		LinkAnimClassLayers(WeaponDefinition->AnimClassLayers);
	}
	if (HasAuthority() && WeaponDefinition->AbilitySets.Num() > 0)
	{
		GiveAbilitySetsToOwner(WeaponDefinition->AbilitySets);
	}
}

void UDungeonRealmsWeaponInstance::AddInputMappingContext(const UInputMappingContext* InputMappingContext, int32 Priority)
{
	const APlayerController* PlayerController = GetPawn()->GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(InputMappingContext, Priority);
		bInputMappingContextAdded = true;
	}
}

void UDungeonRealmsWeaponInstance::LinkAnimClassLayers(TSubclassOf<UAnimInstance> AnimClassLayers)
{
	const ACharacter* OwningCharacter = Cast<ACharacter>(GetPawn());
	if (IsValid(OwningCharacter))
	{
		OwningCharacter->GetMesh()->LinkAnimClassLayers(AnimClassLayers);
		bAnimClassLayersLinked = true;
	}
}

void UDungeonRealmsWeaponInstance::GiveAbilitySetsToOwner(const TArray<UDungeonRealmsAbilitySet*>& AbilitySets)
{
	UAbilitySystemComponent* OwningAbilitySystem =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!IsValid(OwningAbilitySystem))
	{
		return;
	}
		
	for (const UDungeonRealmsAbilitySet* AbilitySet : AbilitySets)
	{
		if (!IsValid(AbilitySet))
		{
			continue;
		}
		AbilitySet->GiveToAbilitySystem(
			OwningAbilitySystem,
			this,
			GrantedHandles
		);
	}
}

void UDungeonRealmsWeaponInstance::OnUnequipped()
{
	const UDungeonRealmsWeaponDefinition* WeaponDefinition = GetDefault<UDungeonRealmsWeaponDefinition>(GetEquipmentDefinition());

	if (bInputMappingContextAdded)
	{
		RemoveInputMappingContext(WeaponDefinition->InputMappingContext);
	}

	if (bAnimClassLayersLinked)
	{
		UnlinkAnimClassLayers(WeaponDefinition->AnimClassLayers);
	}

	if (GrantedHandles.Num() > 0)
	{
		TakeAbilitySetsFromOwner();
	}
	
	Super::OnUnequipped();
}

void UDungeonRealmsWeaponInstance::RemoveInputMappingContext(const UInputMappingContext* InputMappingContext)
{
	const APlayerController* PlayerController =
		GetPawn()->GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->RemoveMappingContext(InputMappingContext);
		bInputMappingContextAdded = false;
	}
}

void UDungeonRealmsWeaponInstance::UnlinkAnimClassLayers(TSubclassOf<UAnimInstance> AnimClassLayers)
{
	const ACharacter* OwningCharacter = Cast<ACharacter>(GetPawn());
	if (IsValid(OwningCharacter))
	{
		OwningCharacter->GetMesh()->UnlinkAnimClassLayers(AnimClassLayers);
		bAnimClassLayersLinked = false;
	}
}

void UDungeonRealmsWeaponInstance::TakeAbilitySetsFromOwner()
{
	UAbilitySystemComponent* AbilitySystemComponent =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (IsValid(AbilitySystemComponent))
	{
		GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
	}
}