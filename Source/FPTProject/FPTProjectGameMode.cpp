// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPTProjectGameMode.h"
#include "FPTProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPTProjectGameMode::AFPTProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
