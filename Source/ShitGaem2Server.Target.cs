// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShitGaem2ServerTarget : TargetRules //Change this line according to the name of your project
{
    public ShitGaem2ServerTarget(TargetInfo Target) : base(Target) //Change this line according to the name of your project
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("ShitGaem2"); //Change this line according to the name of your project
    }
}