// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Operration_DDT : ModuleRules
{
	public Operration_DDT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "NavigationSystem", "UMG", "MotionWarping","PhysicsCore", "HTTP", "Json", "JsonUtilities","GameplayTags","AIModule", "Blutility", "StateTreeModule" });

		// Include paths for Utilities and Global.h
		PublicIncludePaths.AddRange(new string[] { "Source/Operration_DDT/Public" });
		PrivateIncludePaths.AddRange(new string[] { "Source/Operration_DDT/Private" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
