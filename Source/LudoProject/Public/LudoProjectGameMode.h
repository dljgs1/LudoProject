// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameLogic/LudoRoute.h"
#include "GameLogic/PieceCharacter.h"
#include "LudoProjectBlock.h"
#include "LudoProjectGameMode.generated.h"

/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class ALudoProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Ludo, EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ALudoProjectBlock> RouteClass;
	UPROPERTY(Category = Ludo, EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APieceCharacter> PieceClass;
	// 棋子高度
	UPROPERTY(Category = Ludo, EditAnywhere, BlueprintReadWrite)
		float pieceHeight = 50.0f;
	// 棋子高度
	UPROPERTY(Category = Ludo, EditAnywhere, BlueprintReadWrite)
		uint8 MaxPlayerNum = 4;


	/// <summary>
	/// 
	/// </summary>
	TArray<TArray<int32>> MapArray; // 地图
	TMap<int32, ULudoRoute*> RouteMap; // 路线总图
	TMap<uint8, ULudoRoute*> StartPoints; //开始点
	TMap<uint8, TArray<ULudoRoute*>> Parkings; // 停机坪
	int32 MapWidth;
	ALudoProjectGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	ULudoRoute* GetRoute(int32 x, int32 y);

};
