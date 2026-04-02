// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FP_InteractionSystem : ModuleRules
{
	public FP_InteractionSystem(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"FP_InteractionSystem",
			"FP_InteractionSystem/Variant_Horror",
			"FP_InteractionSystem/Variant_Horror/UI",
			"FP_InteractionSystem/Variant_Shooter",
			"FP_InteractionSystem/Variant_Shooter/AI",
			"FP_InteractionSystem/Variant_Shooter/UI",
			"FP_InteractionSystem/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
