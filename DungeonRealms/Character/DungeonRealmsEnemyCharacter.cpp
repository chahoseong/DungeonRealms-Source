#include "Character/DungeonRealmsEnemyCharacter.h"
#include "AbilitySystem/DungeonRealmsAbilitySystemComponent.h"
#include "AbilitySystem/DungeonRealmsAttributeSet.h"
#include "Animation/DungeonRealmsAnimInstance.h"
#include "CombatSystem/DungeonRealmsCombatSystemComponent.h"

ADungeonRealmsEnemyCharacter::ADungeonRealmsEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UDungeonRealmsAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UDungeonRealmsAttributeSet>(TEXT("AttributeSet"));

	CombatSystemComponent = CreateDefaultSubobject<UDungeonRealmsCombatSystemComponent>(TEXT("CombatSystemComponent"));
}

void ADungeonRealmsEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (UDungeonRealmsAnimInstance* DungeonRealmsAnimInstance =
		Cast<UDungeonRealmsAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		DungeonRealmsAnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
	}
}
