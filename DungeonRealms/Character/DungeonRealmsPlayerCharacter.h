#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsCharacter.h"
#include "DungeonRealmsPlayerCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsPlayerCharacter : public ADungeonRealmsCharacter
{
	GENERATED_BODY()

public:
	ADungeonRealmsPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	
protected:
	virtual void BeginPlay() override;
};
