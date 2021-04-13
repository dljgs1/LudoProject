// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LudoGameConfig.generated.h"

UCLASS(Config=Game)
class ULudoGameConfig : public UObject
{
GENERATED_BODY()
public:
	UPROPERTY(Config)
		int32 DirectionCode = 0;
	UPROPERTY(Config)
		int32 P1Start = 0;
	UPROPERTY(Config)
		int32 P2Start = 0;
	UPROPERTY(Config)
		int32 P3Start = 0;
	UPROPERTY(Config)
		int32 P4Start = 0;
};