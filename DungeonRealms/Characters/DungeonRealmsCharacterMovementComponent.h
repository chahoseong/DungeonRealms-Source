#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "DungeonRealmsCharacterMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FDungeonRealmsCharacterGroundInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHit;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance = 0.0f;
	
	uint64 LastUpdateFrame = 0;
};

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UDungeonRealmsCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	const FDungeonRealmsCharacterGroundInfo& GetGroundInfo() const;

private:
	mutable FDungeonRealmsCharacterGroundInfo CachedGroundInfo;
};
