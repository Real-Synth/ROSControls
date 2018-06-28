// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ROSControls : ModuleRules
{
	public ROSControls(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
				"ROSControls/Public"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"ROSControls/Private",
                "../../ROSIntegration/Source/ROSIntegration/Private",
                "../../ROSIntegration/Source/ROSIntegration/Private/rosbridge2cpp"
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "PhysXVehicles",
                "ROSIntegration",
                "UnrealEnginePython"
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "CinematicCamera"
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
