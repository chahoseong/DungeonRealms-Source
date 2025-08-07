#include "Player/DungeonRealmsPlayerController.h"
#include "Character/DungeonRealmsPlayerCharacter.h"
#include "DungeonRealmsGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/DungeonRealmsAbilitySystemComponent.h"
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
	DungeonRealmsInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputHeld, &ThisClass::Input_AbilityInputReleased);
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

void ADungeonRealmsPlayerController::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	if (UDungeonRealmsAbilitySystemComponent* AbilitySystemComponent = UDungeonRealmsAbilitySystemComponent::FindAbilitySystemComponent(GetPawn()))
	{
		AbilitySystemComponent->AbilityInputPressed(InputTag);
	}
}

void ADungeonRealmsPlayerController::Input_AbilityInputHeld(FGameplayTag InputTag)
{
	if (UDungeonRealmsAbilitySystemComponent* AbilitySystemComponent = UDungeonRealmsAbilitySystemComponent::FindAbilitySystemComponent(GetPawn()))
	{
		AbilitySystemComponent->AbilityInputHeld(InputTag);
	}
}

void ADungeonRealmsPlayerController::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	if (UDungeonRealmsAbilitySystemComponent* AbilitySystemComponent = UDungeonRealmsAbilitySystemComponent::FindAbilitySystemComponent(GetPawn()))
	{
		AbilitySystemComponent->AbilityInputReleased(InputTag);
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

void ADungeonRealmsPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UDungeonRealmsAbilitySystemComponent* AbilitySystemComponent = UDungeonRealmsAbilitySystemComponent::FindAbilitySystemComponent(GetPawn()))
	{
		AbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
