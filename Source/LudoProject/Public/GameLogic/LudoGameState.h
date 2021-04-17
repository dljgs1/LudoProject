// Copyright Epic Games, Inc. All Rights Reserved.
// �洢��Ϸ״̬ ����������ɫ�����ӡ�
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameLogic/PieceCharacter.h"
#include "LudoGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	EEmpty = 0,
	EStart = 1, // ��ʼ
	EDice = 2, // �ȴ�ҡ��
	EWaitChoice = 3, // �ȴ��û�ѡ��
	EAI = 4, // 
	EWin = 5,
	EBusy = 6, // æµ�����ɲ�����
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
	// ��ʼһ����Ϸ
	UFUNCTION(BlueprintCallable)
	void StartGame(int32 PlayerNum);

	// ÿ�غϸ���״̬
	void UpdateState(EGameState NewState);

	// �Ƿ�������״̬ ���Խ����²���
	UFUNCTION(BlueprintCallable)
		bool IsInIdle();
	// �Ƿ���������
	UFUNCTION(BlueprintCallable)
		bool IsInPlayerTurn();

	// �����¸���ɫ
	UFUNCTION(BlueprintCallable)
		void TurnPlayer();
	// ѡ������
	UFUNCTION(BlueprintCallable)
		bool PickPiece(APieceCharacter* Piece);
	// ������ǰ����
	UFUNCTION(BlueprintCallable)
		bool GoPiece();

	// ����ҡɫ����
	UFUNCTION(BlueprintCallable)
		void SetDice(uint8 step);

	// �������� �����ǻ��˻��Ǽ�����
	UFUNCTION(BlueprintCallable)
		void OnFlyDone();


	//////////////////// AI������ �������� AI ֻ��Ҫ������������ƶ����Ӽ��� ����Ҫ����
	
	// ��ȡͣ������
	UFUNCTION(BlueprintCallable)
		TArray<APieceCharacter*> GetParkingPieces();

	// ��ȡ��������
	UFUNCTION(BlueprintCallable)
		TArray<APieceCharacter*> GetUsefulPieces();





	/////////////////// �¼�


	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStateUpdate);

	UPROPERTY(BlueprintAssignable)
		FStateUpdate OnStateUpdate;

	UPROPERTY(BlueprintAssignable)
		FStateUpdate OnAIAction;

};



