// Copyright Epic Games, Inc. All Rights Reserved.
/*
* 路径类
*/

#pragma once
#include "LudoProjectBlock.h"
#include "CoreMinimal.h"
#include "LudoRoute.generated.h"

UENUM()
enum class ERouteType : int32
{
	EEmpty = 0,
	ERoute = 1,
	EParking = 2,
	ETransStart =3, // 初始点（允许叠子）
	ETransFinal = 4, // 主路换到终点线的接驳点
	EFinalP1 = 5,
	EFinalP2= 6,
	EFinalP3 = 7,
	EFinalP4 = 8,
	EFinal = 9,
};

/**  */
UCLASS()
class ULudoRoute: public UObject
{
GENERATED_BODY()
public:
	int32 x, y;
	ERouteType Type;
	TArray<ULudoRoute* > Next;
	ALudoProjectBlock* Block;
	ULudoRoute();
	ULudoRoute(int32 _x, int32 _y, ERouteType _type);
	void SetInfo(int32 _x, int32 _y, ERouteType _type);
	void AddRoute(ULudoRoute*);
	void BindBlock(ALudoProjectBlock*);
};
