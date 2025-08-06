#include "Animation/DungeonRealmsAnimInstance.h"
#include "Character/DungeonRealmsCharacterMovementComponent.h"
#include "GameFramework/Character.h"

UDungeonRealmsAnimInstance::UDungeonRealmsAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UDungeonRealmsAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const ACharacter* OwningCharacter =
		Cast<ACharacter>(GetOwningActor());
	if (IsValid(OwningCharacter))
	{
		const UDungeonRealmsCharacterMovementComponent* CharacterMovement = CastChecked<UDungeonRealmsCharacterMovementComponent>(OwningCharacter->GetCharacterMovement());
		const FDungeonRealmsCharacterGroundInfo& GroundInfo = CharacterMovement->GetGroundInfo();
		GroundDistance = GroundInfo.GroundDistance;
	}
}
