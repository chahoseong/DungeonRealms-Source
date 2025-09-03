#include "AbilitySystem/DungeonRealmsGameplayEffectContext.h"

FDungeonRealmsGameplayEffectContext* FDungeonRealmsGameplayEffectContext::ExtraEffectContext(
	FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if (BaseEffectContext->GetScriptStruct()->IsChildOf(StaticStruct()))
	{
		return static_cast<FDungeonRealmsGameplayEffectContext*>(BaseEffectContext);
	}
	return nullptr;
}

FDungeonRealmsGameplayEffectContext::FDungeonRealmsGameplayEffectContext()
{
}

FDungeonRealmsGameplayEffectContext::FDungeonRealmsGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
	: Super(InInstigator, InEffectCauser)
{
}

FGameplayEffectContext* FDungeonRealmsGameplayEffectContext::Duplicate() const
{
	FDungeonRealmsGameplayEffectContext* NewContext = new FDungeonRealmsGameplayEffectContext();
	*NewContext = *this;
	if (GetHitResult())
	{
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}

UScriptStruct* FDungeonRealmsGameplayEffectContext::GetScriptStruct() const
{
	return StaticStruct();
}

bool FDungeonRealmsGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bHasDamageImpact)
		{
			RepBits |= 1 << 7;
		}
		if (bHasKnockbackPower)
		{
			RepBits |= 1 << 8;
		}
		if (bAttackBlocked)
		{
			RepBits |= 1 << 9;
		}
		if (bKnockdown)
		{
			RepBits |= 1 << 10;
		}
	}

	Ar.SerializeBits(&RepBits, 11);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << DamageImpact;
		bHasDamageImpact = true;
	}
	else
	{
		bHasDamageImpact = false;
	}
	if (RepBits & (1 << 8))
	{
		Ar << KnockbackPower;
		bHasKnockbackPower = true;
	}
	else
	{
		bHasKnockbackPower = false;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bAttackBlocked;
	}
	if (RepBits & (1 << 10))
	{
		Ar << bKnockdown;
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	
	
	bOutSuccess = true;
	return true;
}

void FDungeonRealmsGameplayEffectContext::SetDamageImpact(float Value)
{
	DamageImpact = Value;
	bHasDamageImpact = true;
}

float FDungeonRealmsGameplayEffectContext::GetDamageImpact() const
{
	return DamageImpact;
}

void FDungeonRealmsGameplayEffectContext::SetKnockbackPower(float Value)
{
	KnockbackPower = Value;
	bHasKnockbackPower = true;
}

float FDungeonRealmsGameplayEffectContext::GetKnockbackPower() const
{
	return KnockbackPower;
}

void FDungeonRealmsGameplayEffectContext::SetAttackBlocked(bool bValue)
{
	bAttackBlocked = bValue;
}

bool FDungeonRealmsGameplayEffectContext::IsAttackBlocked() const
{
	return bAttackBlocked;
}

void FDungeonRealmsGameplayEffectContext::SetKnockdown(bool bValue)
{
	bKnockdown = bValue;
}

bool FDungeonRealmsGameplayEffectContext::ShouldKnockdown() const
{
	return bKnockdown;
}
