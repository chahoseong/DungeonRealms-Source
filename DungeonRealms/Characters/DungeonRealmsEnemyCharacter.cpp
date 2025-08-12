#include "Characters/DungeonRealmsEnemyCharacter.h"

#include "DungeonRealmsGameplayTags.h"
#include "DungeonRealmsLogChannels.h"
#include "AbilitySystem/DungeonRealmsAbilitySystemComponent.h"
#include "AbilitySystem/DungeonRealmsAttributeSet.h"
#include "AbilitySystem/Data/DungeonRealmsAttributeDefinition.h"
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

	if (HasAuthority())
	{
		InitializeAttributes();
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (UDungeonRealmsAnimInstance* DungeonRealmsAnimInstance =
		Cast<UDungeonRealmsAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		DungeonRealmsAnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
	}
}

void ADungeonRealmsEnemyCharacter::InitializeAttributes()
{
	TArray<FDungeonRealmsAttributeDefinition*> AttributeDefinitions; 
	EnemyAttributesTable->GetAllRows(TEXT(""), AttributeDefinitions);

	const FDungeonRealmsAttributeDefinition* Row = nullptr;
	for (int32 i = 0; i < AttributeDefinitions.Num(); ++i)
	{
		if (AttributeDefinitions[i]->Id == EnemyId)
		{
			Row = AttributeDefinitions[i];
			break;
		}
	}

	if (!Row)
	{
		UE_LOG(LogDungeonRealms, Error, TEXT("Enemy id [%d] not found in [%s]"), EnemyId, *GetPathNameSafe(EnemyAttributesTable));
		return;
	}

	// Primary
	const TMap<FGameplayTag, float> PrimaryAttributeMagnitudes = Row->GetPrimaryAttributeMagnitudes();
	ApplyEffectToSelf(DefaultPrimaryAttributes, PrimaryAttributeMagnitudes);

	// Secondary
	const TMap<FGameplayTag, float> SecondaryAttributeMagnitudes = Row->GetSecondaryAttributeMagnitudes();
	ApplyEffectToSelf(DefaultSecondaryAttributes, SecondaryAttributeMagnitudes);

	// Vital
	ApplyEffectToSelf(DefaultVitalAttributes, TMap<FGameplayTag, float>());
}
