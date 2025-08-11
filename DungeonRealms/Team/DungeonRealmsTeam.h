#pragma once

#include "GenericTeamAgentInterface.h"

UENUM(BlueprintType)
enum class EDungeonRealmsTeam : uint8
{
	Player,
	Enemy,
	NoTeam = 255,
};

FGenericTeamId ToGenericTeamId(EDungeonRealmsTeam Team);
EDungeonRealmsTeam ToDungeonRealmsTeam(FGenericTeamId TeamId);
