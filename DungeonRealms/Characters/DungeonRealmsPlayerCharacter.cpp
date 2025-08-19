#include "Characters/DungeonRealmsPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "Equipment/DungeonRealmsEquipmentManagerComponent.h"
#include "InputActionValue.h"
#include "Animation/DungeonRealmsAnimInstance.h"
#include "CombatSystem/DungeonRealmsCombatSystemComponent.h"
#include "CombatSystem/DungeonRealmsTargetLockComponent.h"
#include "Player/DungeonRealmsPlayerState.h"

ADungeonRealmsPlayerCharacter::ADungeonRealmsPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EquipmentManagerComponent = CreateDefaultSubobject<UDungeonRealmsEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));
	CombatSystemComponent = CreateDefaultSubobject<UDungeonRealmsCombatSystemComponent>(TEXT("CombatSystemComponent"));
	TargetLockComponent = CreateDefaultSubobject<UDungeonRealmsTargetLockComponent>(TEXT("TargetLockComponent"));
}

void ADungeonRealmsPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitializeAbilitySystem();
	InitializeAbilitySets();

	for (TSubclassOf Equipment : StartupEquipments)
	{
		EquipmentManagerComponent->Equip(Equipment);
	}
}

void ADungeonRealmsPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitializeAbilitySystem();

	ADungeonRealmsPlayerState* DungeonRealmsPlayerState = GetPlayerStateChecked<ADungeonRealmsPlayerState>();
	GetMesh()->SetMaterial(0, CharacterMaterials[DungeonRealmsPlayerState->GetPlayerIndex()]);
}

void ADungeonRealmsPlayerCharacter::InitializeAbilitySystem()
{
	ADungeonRealmsPlayerState* DungeonRealmsPlayerState = GetPlayerStateChecked<ADungeonRealmsPlayerState>();

	AbilitySystemComponent = DungeonRealmsPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(DungeonRealmsPlayerState, this);

	if (UDungeonRealmsAnimInstance* DungeonRealmsAnimInstance = Cast<UDungeonRealmsAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		DungeonRealmsAnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
	}

	AttributeSet = DungeonRealmsPlayerState->GetAttributeSet();
}

void ADungeonRealmsPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

UDungeonRealmsCombatSystemComponent* ADungeonRealmsPlayerCharacter::GetCombatSystemComponent() const
{
	return CombatSystemComponent;
}

void ADungeonRealmsPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	if (const AController* OwningController = GetController(); IsValid(OwningController))
	{
		const FVector2D MovementInput = InputActionValue.Get<FVector2D>();
		const FRotator TargetRotation(0.0f, OwningController->GetControlRotation().Yaw, 0.0f);

		if (MovementInput.X != 0.0f)
		{
			const FVector RightDirection = TargetRotation.RotateVector(FVector::RightVector);
			AddMovementInput(RightDirection, MovementInput.X);
		}

		if (MovementInput.Y != 0.0f)
		{
			const FVector ForwardDirection = TargetRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(ForwardDirection, MovementInput.Y);
		}
	}
}

void ADungeonRealmsPlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookInput = InputActionValue.Get<FVector2D>();
	if (LookInput.X != 0.0f)
	{
		AddControllerYawInput(LookInput.X);
	}
	if (LookInput.Y != 0.0f)
	{
		AddControllerPitchInput(LookInput.Y);
	}
}
