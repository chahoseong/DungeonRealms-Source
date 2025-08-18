#include "Characters/DungeonRealmsCharacter.h"

#include "AbilitySystemComponent.h"
#include "CombatSystem/DungeonRealmsCombatStatics.h"
#include "DungeonRealmsCharacterMovementComponent.h"
#include "DungeonRealmsLogChannels.h"
#include "GameplayEffectTypes.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Net/UnrealNetwork.h"
#include "Team/DungeonRealmsTeam.h"

ADungeonRealmsCharacter::ADungeonRealmsCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDungeonRealmsCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;
}

void ADungeonRealmsCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, MyTeam);
}

void ADungeonRealmsCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(NewController))
	{
		MyTeam = ToDungeonRealmsTeam(TeamAgent->GetGenericTeamId());
	}
}

void ADungeonRealmsCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	UE_LOG(LogDungeonRealms, Error, TEXT("You can't set the team id on a possessed character (%s); it's driven by the associated controller"), *GetPathNameSafe(this));
}

FGenericTeamId ADungeonRealmsCharacter::GetGenericTeamId() const
{
	return ToGenericTeamId(MyTeam);
}

ETeamAttitude::Type ADungeonRealmsCharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	return UDungeonRealmsCombatStatics::GetTeamAttitudeTowards(this, &Other);
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

FActiveGameplayEffectHandle ADungeonRealmsCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectToApply,
                                                                const TMap<FGameplayTag, float>& SetByCallers)
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(EffectToApply, 1.0f, EffectContext);
	for (const auto& Pair : SetByCallers)
	{
		EffectSpec.Data->SetSetByCallerMagnitude(Pair.Key, Pair.Value);
	}
	return AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data, GetAbilitySystemComponent());
}

void ADungeonRealmsCharacter::RemoveActiveEffect(const FActiveGameplayEffectHandle& ActiveEffectHandle)
{
	AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
}

void ADungeonRealmsCharacter::AddAnimMontage(const FGameplayTag& MontageTag, UAnimMontage* AnimMontage)
{
	AnimMontages.Add(MontageTag, AnimMontage);
}

void ADungeonRealmsCharacter::RemoveAnimMontage(const FGameplayTag& MontageTag)
{
	AnimMontages.Remove(MontageTag);
}

UAnimMontage* ADungeonRealmsCharacter::GetAnimMontage(const FGameplayTag& MontageTag) const
{
	return AnimMontages.FindRef(MontageTag);
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
