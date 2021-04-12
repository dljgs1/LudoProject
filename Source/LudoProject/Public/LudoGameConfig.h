// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LudoGameConfig.generated.h"

UCLASS(Config=Game)
class ALudoGameConfig : public AActor
{
GENERATED_BODY()
public:
	UPROPERTY(Config)
		int32 DirectionCode=0;
		ALudoGameConfig();
};