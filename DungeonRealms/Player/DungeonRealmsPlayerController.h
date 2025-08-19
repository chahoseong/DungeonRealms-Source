#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "Team/DungeonRealmsTeam.h"
#include "DungeonRealmsPlayerController.generated.h"

class UDungeonRealmsInputConfig;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADungeonRealmsPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	void SetPlayerIndex(int32 NewPlayerIndex);
	
	//~Begin IGenericTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;
	//~End IGenericTeamAgentInterface interface
	
protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

private:
	void SetupOverlay() const;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_SwitchTarget_Triggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTarget_Completed(const FInputActionValue& InputActionValue);
	
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	void Input_AbilityInputHeld(FGameplayTag InputTag);
	void Input_AbilityInputReleased(FGameplayTag InputTag);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UDungeonRealmsInputConfig> InputConfig;

	UPROPERTY(VisibleInstanceOnly, Transient, Category="Team")
	EDungeonRealmsTeam MyTeam = EDungeonRealmsTeam::Player;
	
private:
	FVector2D TargetSwitchingDirection;
	mutable bool bOverlayInitialized = false;
};
