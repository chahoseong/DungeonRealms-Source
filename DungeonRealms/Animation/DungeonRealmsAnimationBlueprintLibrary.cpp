#include "Animation/DungeonRealmsAnimationBlueprintLibrary.h"

bool UDungeonRealmsAnimationBlueprintLibrary::IsPlayInPreview(UObject* WorldContextObject)
{
	const UWorld* World = WorldContextObject->GetWorld();
	return IsValid(World) ? World->IsPlayInPreview() : false;
}
