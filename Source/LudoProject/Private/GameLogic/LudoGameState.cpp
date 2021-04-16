// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameLogic/LudoGameState.h"
#include "GameLogic/PieceCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LudoProjectGameMode.h"

ALudoGameState::ALudoGameState()
{

}

void ALudoGameState::StartGame(int32 PlayerNum)
{
	CurState = EGameState::EStart;
	for (int32 i = 0; i < Pieces.Num(); i++)
	{
		Pieces[i]->K2_DestroyActor();
	}

	ALudoProjectGameMode* CurGameMode = Cast<ALudoProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!CurGameMode)
	{
		return;
	}
	FVector UpBias(0.0, 0.0, CurGameMode->pieceHeight);

	for (TMap<uint8, TArray<ULudoRoute*>>::TConstIterator iter(CurGameMode->Parkings);iter;++iter)
	{
		for (const ULudoRoute* Ptr : iter->Value)
		{
			if (Ptr->Block)
			{
				APieceCharacter* Temp = GetWorld()->SpawnActor<APieceCharacter>(CurGameMode->PieceClass,
					Ptr->Block->GetActorLocation() + UpBias,
					FRotator(0, 0, 0));
				if (Temp)
				{
					Temp->Init(Ptr->x, Ptr->y, iter->Key);
					Pieces.Add(Temp);
				}
			}
		}
	}
}

bool ALudoGameState::IsInIdle()
{
	for (int32 i = 0;i< Pieces.Num();i++)
	{
		if (Pieces[i]->state == EPieceState::EFlying)
		{
			return false;
		}
	}
	return true;
}

void ALudoGameState::TurnPlayer()
{
	ALudoProjectGameMode* CurGameMode = Cast<ALudoProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurGameMode)
	{
		CurPlayer++;
		CurPlayer = CurPlayer % CurGameMode->MaxPlayerNum;
	}
	CurPiece = nullptr;
	// todo 自动选子
}

bool ALudoGameState::PickPiece(int32 x, int32 y)
{
	for (int32 i = 0; i < Pieces.Num(); i++)
	{
		if (Pieces[i]->x == x && Pieces[i]->y == y)
		{
			CurPiece = Pieces[i];
			return true;
		}
	}
	return false;
}
