#include "Player/DungeonRealmsPlayerController.h"
#include "Character/DungeonRealmsPlayerCharacter.h"
#include "DungeonRealmsGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "Input/DungeonRealmsInputComponent.h"

ADungeonRealmsPlayerController::ADungeonRealmsPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ADungeonRealmsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UDungeonRealmsInputComponent* DungeonRealmsInputComponent = CastChecked<UDungeonRealmsInputComponent>(InputComponent);
	DungeonRealmsInputComponent->BindNativeAction(InputConfig, DungeonRealmsGameplayTags::Input_Action_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
	DungeonRealmsInputComponent->BindNativeAction(InputConfig, DungeonRealmsGameplayTags::Input_Action_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look, false);
}

void ADungeonRealmsPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	ADungeonRealmsPlayerCharacter* DungeonRealmsPlayerCharacter =
		GetPawn<ADungeonRealmsPlayerCharacter>();
	if (IsValid(DungeonRealmsPlayerCharacter))
	{
		DungeonRealmsPlayerCharacter->Input_Move(InputActionValue);
	}
}

void ADungeonRealmsPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	ADungeonRealmsPlayerCharacter* DungeonRealmsPlayerCharacter =
		GetPawn<ADungeonRealmsPlayerCharacter>();
	if (IsValid(DungeonRealmsPlayerCharacter))
	{
		DungeonRealmsPlayerCharacter->Input_Look(InputActionValue);
	}
}

void ADungeonRealmsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(DefaultInputMapping, 0);
	}
}
