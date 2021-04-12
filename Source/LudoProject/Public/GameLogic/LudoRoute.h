// Copyright Epic Games, Inc. All Rights Reserved.
/*
* 路径类
*/

#pragma once

#include "CoreMinimal.h"
#include "LudoRoute.generated.h"

UENUM()
enum class ERouteType : int32
{
	EEmpty = 0,
	ERoute = 1,
	EParking = 2,
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
	ULudoRoute();
	ULudoRoute(int32 _x, int32 _y, ERouteType _type);
	void SetInfo(int32 _x, int32 _y, ERouteType _type);
	void AddRoute(ULudoRoute*);
};
