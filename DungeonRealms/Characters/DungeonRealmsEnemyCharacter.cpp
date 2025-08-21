#include "Characters/DungeonRealmsEnemyCharacter.h"
#include "AbilitySystem/DungeonRealmsAbilitySystemComponent.h"
#include "AbilitySystem/DungeonRealmsAttributeSet.h"
#include "AbilitySystem/Data/DungeonRealmsAttributeDefinition.h"
#include "Animation/DungeonRealmsAnimInstance.h"
#include "AI/DungeonRealmsEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatSystem/DungeonRealmsCombatSystemComponent.h"
#include "DungeonRealmsLogChannels.h"
#include "Net/UnrealNetwork.h"

ADungeonRealmsEnemyCharacter::ADungeonRealmsEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UDungeonRealmsAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UDungeonRealmsAttributeSet>(TEXT("AttributeSet"));

	CombatSystemComponent = CreateDefaultSubobject<UDungeonRealmsCombatSystemComponent>(TEXT("CombatSystemComponent"));
}

void ADungeonRealmsEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpawnedWeapons);
}

void ADungeonRealmsEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ADungeonRealmsEnemyController* EnemyController = Cast<ADungeonRealmsEnemyController>(NewController);
	EnemyController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	EnemyController->RunBehaviorTree(BehaviorTree);
}

void ADungeonRealmsEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeRetargetMeshComponent();
	InitializeAbilitySystem();

	if (HasAuthority())
	{
		InitializeAttributes();
		InitializeAbilitySets();
		InitializeWeapons();
	}

	BindAttributeChanges();
	BroadcastInitialAttributes();
}

void ADungeonRealmsEnemyCharacter::Destroyed()
{
	if (HasAuthority())
	{
		DestroySpawnedWeapons();
	}
	Super::Destroyed();
}

void ADungeonRealmsEnemyCharacter::InitializeRetargetMeshComponent()
{
	if (bUseRetargetMesh)
	{
		for (USceneComponent* Child : GetMesh()->GetAttachChildren())
		{
			if (Child->IsA(USkeletalMeshComponent::StaticClass()))
			{
				RetargetMeshComponent = Cast<USkeletalMeshComponent>(Child);
				break;
			}
		}
	}
}

void ADungeonRealmsEnemyCharacter::InitializeAbilitySystem()
{
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

void ADungeonRealmsEnemyCharacter::BindAttributeChanges() const
{
	// Health
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UDungeonRealmsAttributeSet::GetHealthAttribute())
					  .AddLambda([this](const FOnAttributeChangeData& Data) {
						  OnHealthChanged.Broadcast(Data.NewValue);
					  });

	// Max Health
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UDungeonRealmsAttributeSet::GetMaxHealthAttribute())
						  .AddLambda([this](const FOnAttributeChangeData& Data) {
							  OnMaxHealthChanged.Broadcast(Data.NewValue);
						  });
}

void ADungeonRealmsEnemyCharacter::BroadcastInitialAttributes() const
{
	UDungeonRealmsAttributeSet* DungeonRealmsAttributeSet = GetAttributeSet<UDungeonRealmsAttributeSet>();
	OnHealthChanged.Broadcast(DungeonRealmsAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(DungeonRealmsAttributeSet->GetMaxHealth());
}

void ADungeonRealmsEnemyCharacter::InitializeWeapons()
{
	USceneComponent* AttachTarget = GetMesh();
	if (bUseRetargetMesh)
	{
		AttachTarget = RetargetMeshComponent;
	}
	
	for (const FEnemyWeaponToSpawn& WeaponToSpawn : WeaponActorsToSpawn)
	{
		AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(
			WeaponToSpawn.WeaponActorClass,
			FTransform::Identity,
			this
		);
		NewActor->FinishSpawning(FTransform::Identity, true);
		NewActor->SetActorRelativeTransform(WeaponToSpawn.AttachTransform);
		NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, WeaponToSpawn.AttachSocketName);
		
		SpawnedWeapons.Add(NewActor);
	}
}

void ADungeonRealmsEnemyCharacter::DestroySpawnedWeapons()
{
	for (AActor* Weapon : SpawnedWeapons)
	{
		Weapon->Destroy();
	}
	SpawnedWeapons.Reset();
}

AActor* ADungeonRealmsEnemyCharacter::GetAttachedActorFromSocket(FName SocketName) const
{
	if (bUseRetargetMesh)
	{
		for (const USceneComponent* ChildComponent : RetargetMeshComponent->GetAttachChildren())
		{
			if (ChildComponent->GetAttachSocketName() == SocketName)
			{
				return ChildComponent->GetOwner();
			}
		}
		return nullptr;
	}
	else
	{
		return Super::GetAttachedActorFromSocket(SocketName);
	}
}
