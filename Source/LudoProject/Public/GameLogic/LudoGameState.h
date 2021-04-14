// Copyright Epic Games, Inc. All Rights Reserved.
// 存储游戏状态 包括各个角色的棋子、
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameLogic/PieceCharacter.h"
#include "LudoGameState.generated.h"

/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class ALudoGameState : public AGameStateBase
{
	GENERATED_BODY()
	TArray<APieceCharacter*>Pieces;
	uint8 CurPlayer = 0;
	APieceCharacter* CurPiece = nullptr;
public:
	ALudoGameState();
	// 开始一局游戏
	UFUNCTION(BlueprintCallable)
	void StartGame(int32 PlayerNum);

	// 是否处于闲置状态 可以进行新操作
	bool IsInIdle();

	// 激活某个角色
	UFUNCTION(BlueprintCallable)
		void TurnPlayer(uint8 camp);
	// 选择棋子
	UFUNCTION(BlueprintCallable)
		bool PickPiece(int32 x, int32 y);
};



