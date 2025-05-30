// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SPM_Test_NO_LFS : ModuleRules
{
	public SPM_Test_NO_LFS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "UMG", "GameplayTasks", "AIModule", "NavigationSystem", "Paper2D", "DeveloperSettings", "MovieScene", "MovieSceneTracks", "LevelSequence" });

		PrivateDependencyModuleNames.AddRange(new string[] { "CableComponent", "SPM_SGUpgradeSystem", "Paper2D" });

		// Uncomment if you are using Slate UI
		 PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
