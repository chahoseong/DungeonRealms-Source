#include "Character/DungeonRealmsPlayerCharacter.h"
#include "Character/DungeonRealmsCharacterMovementComponent.h"
#include "InputActionValue.h"

ADungeonRealmsPlayerCharacter::ADungeonRealmsPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDungeonRealmsCharacterMovementComponent>(CharacterMovementComponentName))
{
}

void ADungeonRealmsPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
