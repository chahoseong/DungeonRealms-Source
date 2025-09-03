#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonRealmsCombatStatics.generated.h"

struct FDamageSpec;

UCLASS()
class DUNGEONREALMS_API UDungeonRealmsCombatStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Dungeon Realms|Combat")
	static bool HasObstacleBetween(const AActor* SourceActor, const AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category="Dungeon Realms|Combat")
	static bool HasHostileAttitude(const AActor* QueryActor, const AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category="Dungeon Realms|Combat")
	static ETeamAttitude::Type GetTeamAttitudeTowards(const AActor* QueryActor, const AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, Category="Dungeon Realms|Combat")
	static void ApplyDamageEffectToTarget(const AActor* TargetActor, const FDamageSpec& DamageSpec);
};
