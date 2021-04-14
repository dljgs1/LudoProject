// Copyright Epic Games, Inc. All Rights Reserved.
// �洢��Ϸ״̬ ����������ɫ�����ӡ�
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
	// ��ʼһ����Ϸ
	UFUNCTION(BlueprintCallable)
	void StartGame(int32 PlayerNum);

	// �Ƿ�������״̬ ���Խ����²���
	bool IsInIdle();

	// ����ĳ����ɫ
	UFUNCTION(BlueprintCallable)
		void TurnPlayer(uint8 camp);
	// ѡ������
	UFUNCTION(BlueprintCallable)
		bool PickPiece(int32 x, int32 y);
};



