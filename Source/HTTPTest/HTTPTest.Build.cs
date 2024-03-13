// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HTTPTest : ModuleRules
{
	public HTTPTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HTTP", "Json", "JsonUtilities", "UMG" });
	}
}
