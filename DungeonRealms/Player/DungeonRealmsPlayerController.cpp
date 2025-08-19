#include "Player/DungeonRealmsPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/DungeonRealmsAbilitySystemComponent.h"
#include "Characters/DungeonRealmsPlayerCharacter.h"
#include "DungeonRealmsGameplayTags.h"
#include "DungeonRealmsLogChannels.h"
#include "DungeonRealmsPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "CombatSystem/DungeonRealmsCombatStatics.h"
#include "GameFramework/PlayerState.h"
#include "Input/DungeonRealmsInputComponent.h"
#include "UI/DungeonRealmsHUD.h"

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
	DungeonRealmsInputComponent->BindNativeAction(InputConfig, DungeonRealmsGameplayTags::Input_Action_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTarget_Triggered, false);
	DungeonRealmsInputComponent->BindNativeAction(InputConfig, DungeonRealmsGameplayTags::Input_Action_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTarget_Completed, false);
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

void ADungeonRealmsPlayerController::Input_SwitchTarget_Triggered(const FInputActionValue& InputActionValue)
{
	TargetSwitchingDirection = InputActionValue.Get<FVector2D>();
}

void ADungeonRealmsPlayerController::Input_SwitchTarget_Completed(const FInputActionValue& InputActionValue)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetPawn(),
		TargetSwitchingDirection.X > 0.0f ?
		DungeonRealmsGameplayTags::Event_SwitchTarget_Right :
		DungeonRealmsGameplayTags::Event_SwitchTarget_Left,
		FGameplayEventData()
	);
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

void ADungeonRealmsPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsLocalController() && IsValid(PlayerState))
	{
		SetupOverlay();
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

void ADungeonRealmsPlayerController::SetPlayerIndex(int32 NewPlayerIndex)
{
	if (ADungeonRealmsPlayerState* DungeonRealmsPlayerState = GetPlayerState<ADungeonRealmsPlayerState>())
	{
		DungeonRealmsPlayerState->SetPlayerIndex(NewPlayerIndex);
	}
}

void ADungeonRealmsPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (!bOverlayInitialized)
	{
		SetupOverlay();
	}
}

void ADungeonRealmsPlayerController::SetupOverlay() const
{
	ADungeonRealmsHUD* DungeonRealmsHUD = GetHUD<ADungeonRealmsHUD>();
	if (IsValid(DungeonRealmsHUD))
	{
		DungeonRealmsHUD->InitOverlay();
		bOverlayInitialized = true;
	}
}

void ADungeonRealmsPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	UE_LOG(LogDungeonRealms, Error, TEXT("You can't set the team id (%s); This controller's team id have to player."), *GetPathNameSafe(this));
}

FGenericTeamId ADungeonRealmsPlayerController::GetGenericTeamId() const
{
	return ToGenericTeamId(MyTeam);
}

ETeamAttitude::Type ADungeonRealmsPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	return UDungeonRealmsCombatStatics::GetTeamAttitudeTowards(GetPawn(), &Other);
}
