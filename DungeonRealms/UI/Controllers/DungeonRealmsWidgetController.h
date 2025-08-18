#pragma once

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "DungeonRealmsWidgetController.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsWidgetController : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(APlayerController* NewPlayerController);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() const;
	
	UFUNCTION(BlueprintPure)
	APlayerController* GetPlayerController() const;

	template <typename T>
	T* GetPlayerController() const
	{
		return Cast<T>(GetPlayerController());
	}

	UFUNCTION(BlueprintPure)
	APlayerState* GetPlayerState() const;

	template <typename T>
	T* GetPlayerState() const
	{
		return Cast<T>(GetPlayerState());
	}

protected:
	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TWeakObjectPtr<APlayerController> PlayerController;
};
