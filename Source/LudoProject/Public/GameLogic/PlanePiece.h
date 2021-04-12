// Copyright Epic Games, Inc. All Rights Reserved.
/*
棋子数据类
*/

#pragma once

#include "CoreMinimal.h"
// #include "GameFramework/GameStateBase.h"
#include "PlanePiece.generated.h"

/**  */
UCLASS()
class UPlanePiece : public UObject
{
GENERATED_BODY()
public:
	int32 x;
	int32 y;
	int32 direction;
	int32 camp; // 阵营
};
