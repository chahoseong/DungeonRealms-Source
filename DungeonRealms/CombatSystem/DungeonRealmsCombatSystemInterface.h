#pragma once

#include "CoreMinimal.h"

#include "DungeonRealmsCombatSystemInterface.generated.h"

class UDungeonRealmsCombatSystemComponent;

// This class does not need to be modified.
UINTERFACE()
class UDungeonRealmsCombatSystemInterface : public UInterface
{
	GENERATED_BODY()
};

class DUNGEONREALMS_API IDungeonRealmsCombatSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UDungeonRealmsCombatSystemComponent* GetCombatSystemComponent() const = 0;
};
