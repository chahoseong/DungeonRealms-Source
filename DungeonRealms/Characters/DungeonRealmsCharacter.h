#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/DungeonRealmsAbilitySet.h"
#include "GameFramework/Character.h"
#include "Team/DungeonRealmsTeam.h"
#include "DungeonRealmsCharacter.generated.h"

class UAttributeSet;

UCLASS()
class DUNGEONREALMS_API ADungeonRealmsCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADungeonRealmsCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FActiveGameplayEffectHandle ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectToApply, const TMap<FGameplayTag, float>& SetByCallers);
	void RemoveActiveEffect(const FActiveGameplayEffectHandle& ActiveEffectHandle);
	
	//~Begin APawn interface
	virtual void PossessedBy(AController* NewController) override;
	//~End APawn interface
	
	//~Begin IDungeonRealmsTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~End IDungeonRealmsTeamAgentInterface interface
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	template <typename T>
	T* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}

	UAttributeSet* GetAttributeSet() const;

	template <typename T>
	T* GetAttributeSet() const
	{
		return Cast<T>(GetAttributeSet());
	}

	virtual AActor* GetAttachedActorFromSocket(FName SocketName) const;

protected:
	void InitializeAbilitySets();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DungeonRealms|Abilities")
	TArray<TSoftObjectPtr<UDungeonRealmsAbilitySet>> AbilitySets;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated)
	EDungeonRealmsTeam MyTeam = EDungeonRealmsTeam::NoTeam;
	
private:
	FDungeonRealmsAbilitySetGrantedHandles GrantedHandles;
};
