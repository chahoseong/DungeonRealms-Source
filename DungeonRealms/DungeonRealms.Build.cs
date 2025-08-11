// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DungeonRealms : ModuleRules
{
	public DungeonRealms(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
			"GameplayAbilities",
			"GameplayTasks",
		]);
		
		PublicIncludePaths.AddRange([
			"DungeonRealms"
		]);

		PrivateDependencyModuleNames.AddRange([
			"InputCore",
			"EnhancedInput",
			"NetCore",
			"AIModule"
		]);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
