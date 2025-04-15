// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DOTSProjector : ModuleRules
{
	public DOTSProjector(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "Slate", "SlateCore", "UMG" });
	}
}
