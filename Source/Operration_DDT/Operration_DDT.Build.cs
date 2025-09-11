
using UnrealBuildTool;

public class Operration_DDT : ModuleRules
{
	public Operration_DDT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "NavigationSystem", "UMG", "MotionWarping","PhysicsCore", "HTTP", "Json", "JsonUtilities","GameplayTags","AIModule", "StateTreeModule", "PoseSearch" });

		PublicIncludePaths.AddRange(new string[] { "Source/Operration_DDT/Public" });
		PrivateIncludePaths.AddRange(new string[] { "Source/Operration_DDT/Private" });

	}
}
