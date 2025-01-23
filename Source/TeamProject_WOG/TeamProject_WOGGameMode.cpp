// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeamProject_WOGGameMode.h"
#include "TeamProject_WOGCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATeamProject_WOGGameMode::ATeamProject_WOGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
