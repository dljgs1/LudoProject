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
	// ���Ӹ߶�
	UPROPERTY(Category = Ludo, EditAnywhere, BlueprintReadWrite)
		float pieceHeight = 50.0f;
	// ���Ӹ߶�
	UPROPERTY(Category = Ludo, EditAnywhere, BlueprintReadWrite)
		uint8 MaxPlayerNum = 4;


	/// <summary>
	/// 
	/// </summary>
	TArray<TArray<int32>> MapArray; // ��ͼ
	TMap<int32, ULudoRoute*> RouteMap; // ·����ͼ
	TMap<uint8, ULudoRoute*> StartPoints; //��ʼ��
	TMap<uint8, TArray<ULudoRoute*>> Parkings; // ͣ��ƺ
	int32 MapWidth;
	ALudoProjectGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	ULudoRoute* GetRoute(int32 x, int32 y);

};
