#include "Character/DungeonRealmsCharacter.h"

#include "DungeonRealmsCharacterMovementComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Team/DungeonRealmsTeam.h"

ADungeonRealmsCharacter::ADungeonRealmsCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDungeonRealmsCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ADungeonRealmsCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (IGenericTeamAgentInterface* GenericTeamAgent = Cast<IGenericTeamAgentInterface>(GetController()))
	{
		GenericTeamAgent->SetGenericTeamId(NewTeamID);
	}
}

FGenericTeamId ADungeonRealmsCharacter::GetGenericTeamId() const
{
	if (const IGenericTeamAgentInterface* GenericTeamAgent = Cast<const IGenericTeamAgentInterface>(GetController()))
	{
		return GenericTeamAgent->GetGenericTeamId();
	}
	return ToGenericTeamId(EDungeonRealmsTeam::NoTeam);
}

ETeamAttitude::Type ADungeonRealmsCharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const IGenericTeamAgentInterface* GenericTeamAgent = Cast<const IGenericTeamAgentInterface>(GetController()))
	{
		return GenericTeamAgent->GetTeamAttitudeTowards(Other);
	}
	return ETeamAttitude::Neutral;
}

void ADungeonRealmsCharacter::InitializeAbilitySets()
{
	TArray<FSoftObjectPath> Paths;
	
	for (TSoftObjectPtr<UDungeonRealmsAbilitySet> AbilitySet : AbilitySets)
	{
		if (AbilitySet.IsPending())
		{
			Paths.Add(AbilitySet.ToSoftObjectPath());
		}
		else
		{
			AbilitySet.Get()->GiveToAbilitySystem(
				AbilitySystemComponent,
				this,
				GrantedHandles
			);
		}
	}

	if (Paths.Num() > 0)
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		TWeakObjectPtr ThisCharacter = this;
		Streamable.RequestAsyncLoad(Paths, FStreamableDelegate::CreateLambda([ThisCharacter]()
		{
			if (ThisCharacter.IsValid())
			{
				ThisCharacter->InitializeAbilitySets();
			}
		}));
	}
}

UAbilitySystemComponent* ADungeonRealmsCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ADungeonRealmsCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

AActor* ADungeonRealmsCharacter::GetAttachedActorFromSocket(FName SocketName) const
{
	for (const USceneComponent* ChildComponent : GetMesh()->GetAttachChildren())
	{
		if (ChildComponent->GetAttachSocketName() == SocketName)
		{
			return ChildComponent->GetOwner();
		}
	}
	return nullptr;
}
