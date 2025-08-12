#include "Animation/DungeonRealmsAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Characters/DungeonRealmsCharacterMovementComponent.h"
#include "GameFramework/Character.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UDungeonRealmsAnimInstance::UDungeonRealmsAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UDungeonRealmsAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (const AActor* OwningActor = GetOwningActor(); IsValid(OwningActor))
	{
		UAbilitySystemComponent* AbilitySystem =
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);
		if (IsValid(AbilitySystem))
		{
			InitializeWithAbilitySystem(AbilitySystem);
		}
	}
}

void UDungeonRealmsAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);
	GameplayTagPropertyMap.Initialize(this, AbilitySystem);
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

EDataValidationResult UDungeonRealmsAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);
	GameplayTagPropertyMap.IsDataValid(this, Context);
	return Context.GetNumErrors() > 0 ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
