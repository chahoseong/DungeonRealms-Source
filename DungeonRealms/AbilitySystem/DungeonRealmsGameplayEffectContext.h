#pragma once

#include "GameplayEffectTypes.h"
#include "DungeonRealmsGameplayEffectContext.generated.h"

USTRUCT()
struct FDungeonRealmsGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
	static DUNGEONREALMS_API FDungeonRealmsGameplayEffectContext* ExtraEffectContext(FGameplayEffectContextHandle Handle);

	FDungeonRealmsGameplayEffectContext();
	FDungeonRealmsGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser);

	virtual FGameplayEffectContext* Duplicate() const override;
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	void SetDamageImpact(float Value);
	float GetDamageImpact() const;
	
	void SetKnockbackPower(float Value);
	float GetKnockbackPower() const;
	
	void SetKnockdown(bool bValue);
	bool ShouldKnockdown() const;
	
protected:
	UPROPERTY()
	float DamageImpact = 0.0f;

	UPROPERTY()
	bool bHasDamageImpact = false;
	
	UPROPERTY()
	float KnockbackPower = 0.0f;

	UPROPERTY()
	bool bHasKnockbackPower = false;
	
	UPROPERTY()
	bool bKnockdown = false;
};

template <>
struct TStructOpsTypeTraits<FDungeonRealmsGameplayEffectContext> : TStructOpsTypeTraitsBase2<FDungeonRealmsGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
