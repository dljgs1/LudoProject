// Copyright Epic Games, Inc. All Rights Reserved.

#include "LudoProjectGameMode.h"
#include "LudoProjectPlayerController.h"
#include "LudoProjectPawn.h"

ALudoProjectGameMode::ALudoProjectGameMode()
{
	// no pawn by default
	DefaultPawnClass = ALudoProjectPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ALudoProjectPlayerController::StaticClass();
}
