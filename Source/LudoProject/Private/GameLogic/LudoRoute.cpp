// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameLogic/LudoRoute.h"

ULudoRoute::ULudoRoute(int32 _x, int32 _y, ERouteType _type)
{
	x = _x;
	y = _y;
	Type = _type;
}

ULudoRoute::ULudoRoute()
{
	x = 0;
	y = 0;
	Type = ERouteType::EEmpty;
}

void ULudoRoute::SetInfo(int32 _x, int32 _y, ERouteType _type)
{
	x = _x;
	y = _y;
	Type = _type;
}

void ULudoRoute::AddRoute(ULudoRoute* OtherRoute)
{
	Next.Add(OtherRoute);
}

void ULudoRoute::BindBlock(ALudoProjectBlock*TargetBlock)
{
	Block = TargetBlock;
}
