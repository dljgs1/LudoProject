// Copyright Epic Games, Inc. All Rights Reserved.

#include "LudoProjectGameMode.h"
#include "LudoProjectPlayerController.h"
#include "LudoProjectPawn.h"
#include "HAL/PlatformFilemanager.h"
#include "FileHelpers.h"
#include "GameLogic/LudoGameState.h"
#include "LudoGameConfig.h"

ALudoProjectGameMode::ALudoProjectGameMode()
{
	// no pawn by default
	DefaultPawnClass = ALudoProjectPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ALudoProjectPlayerController::StaticClass();

	GameStateClass = ALudoGameState::StaticClass();
}

void ALudoProjectGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	FString TxtStream;
	TArray<int32> NumArray;
	/// 初始化地图信息
	FString TxtPath = (FPaths::ProjectContentDir() + TEXT("MapConfig.txt"));
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
			if (TxtData[i] >= 'a' && TxtData[i] <= 'z')
			{
				int32 Num = -(int32)(TxtData[i] - 'a');
				NumArray.Add(Num);
			}
		}
		Size = (int32)FMath::Sqrt(NumArray.Num());
		for (int i = 0; i < Size; i++)
		{
			TArray<int32> Row;
			for (int j = 0; j < Size; j++)
			{
				Row.Add(NumArray[j + i * Size]);
			}
			MapArray.Add(Row);
		}
		MapWidth = Size;
	}

	
	class VectorHelper
	{
	public:
		int32 x=0, y=0;
		int32 dx=0, dy=0;
		VectorHelper() 
		{
		}
		VectorHelper(int32 _x, int32 _y) :x(_x), y(_y)
		{
		}
		VectorHelper(int32 _x,int32 _y, int32 nex, int32 ney):x(_x),y(_y)
		{
			dx = nex - x;
			dy = ney - y;
		}
		int32 ToIndex(int32 Size)
		{
			return x + y * Size;
		}
		VectorHelper GenNext(int32 _dx, int32 _dy)
		{
			return VectorHelper(x + _dx, y + _dy);
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
	};
	/// DFS初始化拓扑信息
	ALudoGameConfig MyConfig;
	MyConfig.LoadConfig();
	NumArray.Empty();
	int StartCode = MyConfig.DirectionCode;
	int DivNum = 10;
	for (int i = 0; i < 4; i++)
	{
		NumArray.Add(StartCode % DivNum);
		DivNum *= 10;
		StartCode /= 10;
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
		TMap<int32, ULudoRoute*> VisitNodes;
		ULudoRoute* Start = NewObject<ULudoRoute>();
		Start->SetInfo(Cur.x, Cur.y, ERouteType(MapArray[Cur.x][Cur.y]));
		VisitNodes[Cur.ToIndex(MapWidth)] = Start;
		Cur.GoStep();
		Queue.Enqueue(Cur);
		while (!Queue.IsEmpty())
		{
			VectorHelper Next;
			if (Queue.Dequeue(Next))
			{
				ULudoRoute* Temp = NewObject<ULudoRoute>();
				Temp->SetInfo(Next.x, Next.y, ERouteType(MapArray[Next.x][Next.y]));

			}
		}
	} while (false);

}
