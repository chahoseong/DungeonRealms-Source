#include "Team/DungeonRealmsTeam.h"

FGenericTeamId ToGenericTeamId(EDungeonRealmsTeam Team)
{
	return FGenericTeamId(static_cast<uint8>(Team));
}

EDungeonRealmsTeam ToDungeonRealmsTeam(FGenericTeamId TeamId)
{
	const UEnum* TeamEnum = StaticEnum<EDungeonRealmsTeam>();
	return TeamEnum->IsValidEnumValue(TeamId.GetId()) ? static_cast<EDungeonRealmsTeam>(TeamId.GetId()) : EDungeonRealmsTeam::NoTeam;
}
