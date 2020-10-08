// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShitGaem2GameMode.h"
#include "ShitGaem2Character.h"
#include "UObject/ConstructorHelpers.h"

AShitGaem2GameMode::AShitGaem2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
