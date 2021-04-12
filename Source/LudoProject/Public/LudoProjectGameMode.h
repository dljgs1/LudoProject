// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameLogic/LudoRoute.h"
#include "LudoProjectGameMode.generated.h"

/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class ALudoProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()
	TArray<TArray<int32>> MapArray; // µØÍ¼
	TArray<ULudoRoute*> StartPoints;
public:
	int32 MapWidth;
	ALudoProjectGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
};
