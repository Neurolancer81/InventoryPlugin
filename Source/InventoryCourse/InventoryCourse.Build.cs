// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InventoryCourse : ModuleRules
{
	public InventoryCourse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"InventoryCourse",
			"InventoryCourse/Variant_Platforming",
			"InventoryCourse/Variant_Combat",
			"InventoryCourse/Variant_Combat/AI",
			"InventoryCourse/Variant_SideScrolling",
			"InventoryCourse/Variant_SideScrolling/Gameplay",
			"InventoryCourse/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
