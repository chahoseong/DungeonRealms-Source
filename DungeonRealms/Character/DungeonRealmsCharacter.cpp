#include "Character/DungeonRealmsCharacter.h"

ADungeonRealmsCharacter::ADungeonRealmsCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

}

void ADungeonRealmsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
