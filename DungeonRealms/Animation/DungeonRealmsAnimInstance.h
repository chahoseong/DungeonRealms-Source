#pragma once

#include "Animation/AnimInstance.h"
#include "DungeonRealmsAnimInstance.generated.h"

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UDungeonRealmsAnimInstance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1.0f;
};
