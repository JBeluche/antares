// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Antares : ModuleRules
{
	public Antares(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
