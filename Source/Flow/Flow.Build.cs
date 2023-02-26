// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Flow : ModuleRules
{
	public Flow(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", 
            "EnhancedInput", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
			"GameplayAbilities",
			"GameplayTags"
        });

		PrivateDependencyModuleNames.AddRange(new string[]
        {
            "EnhancedInput"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
