// Copyright Epic Games, Inc. All Rights Reserved.
// �洢��Ϸ״̬ ����������ɫ�����ӡ�
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameLogic/PieceCharacter.h"
#include "LudoGameState.generated.h"

UENUM()
enum class EGameState : int32
{
	EEmpty = 0,
	EStart = 1, // ��ʼ
	EDice = 2, // �ȴ�ҡ��
	EWaitChoice = 3, // �ȴ��û�ѡ��
	EAI = 4, // 
	EWin = 5,
	EBusy = 6, // æµ�����ɲ�����
};


/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class ALudoGameState : public AGameStateBase
{
	GENERATED_BODY()
	TArray<APieceCharacter*>Pieces;
	uint8 CurPlayer = 0;
	APieceCharacter* CurPiece = nullptr;
public:
	UPROPERTY(BlueprintReadWrite)
	EGameState CurState;
	ALudoGameState();
	// ��ʼһ����Ϸ
	UFUNCTION(BlueprintCallable)
	void StartGame(int32 PlayerNum);

	// �Ƿ�������״̬ ���Խ����²���
	UFUNCTION(BlueprintCallable)
	bool IsInIdle();

	// �����¸���ɫ
	UFUNCTION(BlueprintCallable)
		void TurnPlayer();
	// ѡ������
	UFUNCTION(BlueprintCallable)
		bool PickPiece(int32 x, int32 y);
};



