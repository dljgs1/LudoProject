// Copyright Epic Games, Inc. All Rights Reserved.
// 存储游戏状态 包括各个角色的棋子、
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameLogic/PieceCharacter.h"
#include "LudoGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	EEmpty = 0,
	EStart = 1, // 开始
	EDice = 2, // 等待摇点
	EWaitChoice = 3, // 等待用户选择
	EAI = 4, // 
	EWin = 5,
	EBusy = 6, // 忙碌（不可操作）
	ELose = 7, // 
};


/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class ALudoGameState : public AGameStateBase
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<APieceCharacter*>Pieces;
	UPROPERTY()
	uint8 CurPlayer = 0;
	APieceCharacter* CurPiece = nullptr;
	UPROPERTY()
		uint8 MainPlayerCamp = 0;

	UPROPERTY()
		TMap<uint8, bool> InitMap;

	bool GetPiece(uint8 x, uint8 y, TArray<APieceCharacter*>& OutArray);

public:
	UPROPERTY(BlueprintReadOnly)
		uint8 CurDiceNum;
	UPROPERTY(BlueprintReadWrite)
		bool bAIActing;

	UPROPERTY(BlueprintReadWrite)
	EGameState CurState;
	ALudoGameState();
	// 开始一局游戏
	UFUNCTION(BlueprintCallable)
	void StartGame(int32 PlayerNum);

	// 每回合更新状态
	void UpdateState(EGameState NewState);

	// 是否处于闲置状态 可以进行新操作
	UFUNCTION(BlueprintCallable)
		bool IsInIdle();
	// 是否处于主角轮
	UFUNCTION(BlueprintCallable)
		bool IsInPlayerTurn();

	// 激活下个角色
	UFUNCTION(BlueprintCallable)
		void TurnPlayer();
	// 选择棋子
	UFUNCTION(BlueprintCallable)
		bool PickPiece(APieceCharacter* Piece);
	// 启动当前棋子
	UFUNCTION(BlueprintCallable)
		bool GoPiece();

	// 设置摇色子数
	UFUNCTION(BlueprintCallable)
		void SetDice(uint8 step);

	// 跑完后调用 决定是换人还是继续跑
	UFUNCTION(BlueprintCallable)
		void OnFlyDone();


	//////////////////// AI工具类 ———— AI 只需要根据骰子随机移动棋子即可 不需要策略
	
	// 获取停机棋子
	UFUNCTION(BlueprintCallable)
		TArray<APieceCharacter*> GetParkingPieces();

	// 获取可用棋子
	UFUNCTION(BlueprintCallable)
		TArray<APieceCharacter*> GetUsefulPieces();





	/////////////////// 事件


	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStateUpdate);

	UPROPERTY(BlueprintAssignable)
		FStateUpdate OnStateUpdate;

	UPROPERTY(BlueprintAssignable)
		FStateUpdate OnAIAction;

};



