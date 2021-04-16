// Copyright Epic Games, Inc. All Rights Reserved.

#include "LudoProjectGameMode.h"
#include "LudoProjectPlayerController.h"
#include "LudoProjectPawn.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "GameLogic/LudoGameState.h"
#include "LudoGameConfig.h"

ALudoProjectGameMode::ALudoProjectGameMode()
{
	// no pawn by default
	DefaultPawnClass = ALudoProjectPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ALudoProjectPlayerController::StaticClass();
	GameStateClass = ALudoGameState::StaticClass();

	RouteClass = ALudoProjectBlock::StaticClass();
	PieceClass = APieceCharacter::StaticClass();
}

class VectorHelper
{
public:
	int32 x = 0, y = 0;
	int32 dx = 0, dy = 0;
	ULudoRoute* pRoute = nullptr;
	VectorHelper()
	{
	}
	VectorHelper(int32 _x, int32 _y) :x(_x), y(_y)
	{
	}
	// 
	VectorHelper(int32 sx, int32 sy, int32 ex, int32 ey) :x(ex), y(ey)
	{
		dx = ex - sx;
		dy = ey - sy;
	}
	int32 ToIndex()
	{
		return x * 100 + y;
	}
	// [Min, Max)
	bool CheckValid(int32 Min, int32 Max)
	{
		return x >= Min && x < Max&& y >= Min && y < Max;
	}
	VectorHelper GenNext(int32 _dx, int32 _dy)
	{
		return VectorHelper(x, y, x + _dx, y + _dy);
	}
	void GoStep()
	{
		x += dx;
		y += dy;
	}
	void SetDirection(int32 _dx, int32 _dy)
	{
		dx = _dx;
		dy = _dy;
	}
	VectorHelper Left()
	{
		return GenNext(dy, dx);
	}
	VectorHelper Right()
	{
		return GenNext(-dy, -dx);
	}
	VectorHelper Forward()
	{
		return GenNext(dx, dy);
	}
};

void ALudoProjectGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	FString TxtStream;
	TArray<int32> NumArray;
	/// 初始化地图信息
	FString TxtPath = (FPaths::ProjectContentDir() + TEXT("Config/MapConfig.txt"));
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*TxtPath))
	{
		int32 Size = 0;
		FFileHelper::LoadFileToString(TxtStream, *TxtPath);
		TCHAR* TxtData = TxtStream.GetCharArray().GetData();
		for (int i = 0; i < TxtStream.Len(); i++)
		{
			if (TxtData[i] >= '0' && TxtData[i] <= '9')
			{
				int32 Num = (int32)(TxtData[i] - '0');
				NumArray.Add(Num);
			}
			// Parking nodes:
			if (TxtData[i] >= 'a' && TxtData[i] <= 'z')
			{
				int32 Num = -(int32)(TxtData[i] - 'a' + 1);
				NumArray.Add(Num);
			}
		}
		Size = (int32)FMath::Sqrt(NumArray.Num());
		for (int32 x = 0; x < Size; x++)
		{
			TArray<int32> Row;
			for (int32 y = 0; y < Size; y++)
			{
				Row.Add(NumArray[x + y * Size]);
			}
			MapArray.Add(Row);
		}
		MapWidth = Size;
	}
	/// DFS初始化拓扑信息
	/// 1 : 主干道 2 : 终点接驳点 
	ULudoGameConfig* MyConfig = NewObject<ULudoGameConfig>();
	MyConfig->LoadConfig();
	NumArray.Empty();
	int StartCode = MyConfig->DirectionCode;
	int DivNum = 100;
	for (int i = 0; i < 4; i++)
	{
		NumArray.Add(StartCode % DivNum);
		StartCode /= DivNum;
	}
	do 
	{
		for (int i = 0; i < NumArray.Num(); i++)
		{
			if (NumArray[i] <0 || NumArray[i] >= MapWidth)
			{
				return;
			}
		}
		VectorHelper Cur(NumArray[3], NumArray[2], NumArray[1], NumArray[0]);
		TQueue<VectorHelper> Queue;
		TMap<int32, ULudoRoute*>& VisitNodes = RouteMap;
		ULudoRoute* Start = NewObject<ULudoRoute>();
		int32 StartIndex = Cur.ToIndex();
		Start->SetInfo(Cur.x, Cur.y, ERouteType(MapArray[Cur.x][Cur.y]));
		VisitNodes.Emplace(StartIndex, Start);
		Cur.GoStep();
		Cur.pRoute = NewObject<ULudoRoute>();
		Start->AddRoute(Cur.pRoute);
		Cur.pRoute->SetInfo(Cur.x, Cur.y, ERouteType(MapArray[Cur.x][Cur.y]));
		Queue.Enqueue(Cur);
		while (!Queue.IsEmpty())
		{
			VectorHelper Last;
			
			if (Queue.Dequeue(Last))
			{
				VisitNodes.Emplace(Last.ToIndex(), Last.pRoute);
				int32 LastNumber = MapArray[Last.x][Last.y];
				if (LastNumber == (int32)ERouteType::EFinal)
				{
					//终点停寻
					continue;
				}
				TArray<VectorHelper> DetectList = { Last.Forward(), Last.Right(), Last.Left()};
				for (int i = 0;i<DetectList.Num();i++)
				{
					VectorHelper& TryNode = DetectList[i];
					if (TryNode.CheckValid(0, MapWidth) && MapArray[TryNode.x][TryNode.y] != 0)
					{
						int32 TryNumber = MapArray[TryNode.x][TryNode.y];
						if (LastNumber == (int32)ERouteType::ERoute && TryNumber != (int32)ERouteType::ERoute && TryNumber != (int32)ERouteType::ETransFinal)
						{
							// 主路只允许链接主路与接驳点
							continue;
						}
						if (LastNumber <= (int32)ERouteType::EFinalP4 && LastNumber >= (int32)ERouteType::EFinalP1)
						{
							// 降落道只能连接终点或者同类降落道
							if(TryNumber != LastNumber || TryNumber != (int32)ERouteType::EFinal)
								continue;
						}
						int32 TryIndex = TryNode.ToIndex();
						if (VisitNodes.Contains(TryIndex))
						{
							if (TryIndex == StartIndex)
							{
								Last.pRoute->AddRoute(Start);
							}
						}
						else
						{
							TryNode.pRoute = NewObject<ULudoRoute>();
							Last.pRoute->AddRoute(TryNode.pRoute);
							TryNode.pRoute->SetInfo(TryNode.x, TryNode.y, ERouteType(MapArray[TryNode.x][TryNode.y]));
							Queue.Enqueue(TryNode);
						}
					}
				}
			}
		}
		NumArray = { MyConfig->P1Start, MyConfig->P2Start, MyConfig->P3Start, MyConfig->P4Start};
		for (int32 i = 0;i<NumArray.Num();i++)
		{
			ULudoRoute* Temp = VisitNodes.FindRef(NumArray[i]);
			if (Temp)
			{
				StartPoints.Emplace(i+1, Temp); // campid : point
				Parkings.Emplace(i + 1, TArray<ULudoRoute*>());
			}
		}
	} while (0);
	// 停机场初始化
	for (int32 x = 0; x < MapWidth; x++)
	{
		for (int32 y = 0; y < MapWidth; y++)
		{
			if (MapArray[x][y] < 0)
			{
				ULudoRoute* Node = NewObject<ULudoRoute>();
				VectorHelper Vec(x, y);
				int32 Number = -MapArray[x][y];
				Node->SetInfo(x, y, ERouteType::EParking);
				RouteMap.Add(Vec.ToIndex(), Node);
				if (StartPoints.Contains(Number))
				{
					Node->AddRoute(*StartPoints.Find(Number));
				}
				if (Parkings.Contains(Number))
				{
					Parkings.Find(Number)->Add(Node);
				}
				MapArray[x][y] = (int32)ERouteType::EParking;
			}
		}
	}
}






ULudoRoute* ALudoProjectGameMode::GetRoute(int32 x, int32 y)
{
	int32 Index = VectorHelper(x, y).ToIndex();
	return RouteMap.FindRef(Index);
}



