// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShitGaem2ClientTarget : TargetRules
{
    public ShitGaem2ClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		BuildEnvironment = TargetBuildEnvironment.Shared;
		ExtraModuleNames.Add("ShitGaem2");
	}
}
