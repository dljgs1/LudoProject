// Copyright Epic Games, Inc. All Rights Reserved.

#include "LudoProjectPlayerController.h"

ALudoProjectPlayerController::ALudoProjectPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
