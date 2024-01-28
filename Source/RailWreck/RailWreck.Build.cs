using UnrealBuildTool;

public class RailWreck : ModuleRules
{
	public RailWreck(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] { "FlowCore", "VRExpansionPlugin", "HeadMountedDisplay" });
	}
}
