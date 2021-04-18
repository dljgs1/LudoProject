// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameLogic/LudoGameState.h"
#include "GameLogic/PieceCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LudoProjectGameMode.h"

ALudoGameState::ALudoGameState()
{

}

void ALudoGameState::SaveState()
{
	PieceState = TArray< int32>();
	for (int32 i = 0; i < Pieces.Num(); i++)
	{
		PieceState.Add((int32)Pieces[i]->state * 10000 + Pieces[i]->x * 100 + Pieces[i]->y);
	}
	LastDiceNum = CurDiceNum;
}

void ALudoGameState::LoadState()
{
	if (PieceState.Num() == 0)return;
	for (int32 i = 0; i < PieceState.Num(); i++)
	{
		int32 Pos = PieceState[i] % 10000;
		int32 x = Pos / 100;
		int32 y = Pos % 100;
		int32 st = PieceState[i] / 10000;
		Pieces[i]->HardReset(x, y, (EPieceState)st, false);
	}
	PieceState.Empty();
	CurPlayer = MainPlayerCamp;
	CurDiceNum = LastDiceNum;
	UpdateState(EGameState::EWaitChoice);
}

bool ALudoGameState::GetPiece(uint8 x, uint8 y, TArray<APieceCharacter*>& OutArray)
{
	bool Ret = false;
	for (auto&Piece : Pieces)
	{
		if (Piece->x == x && Piece->y == y)
		{
			OutArray.Add(Piece);
			Ret = true;
		}
	}
	return Ret;
}
void ALudoGameState::StartGame(int32 PlayerNum)
{
	MainPlayerCamp = 0;
	CurDiceNum = 1;
	InitMap.Empty();
	PieceState.Empty();
	CurState = EGameState::EStart;
	for (int32 i = 0; i < Pieces.Num(); i++)
	{
		Pieces[i]->K2_DestroyActor();
	}
	Pieces.Empty();

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
	for (int32 i = 0; i < Pieces.Num(); i++)
	{
		if (Pieces[i]->state == EPieceState::EFlying)
		{
			return false;
		}
	}
	return true;
}

bool ALudoGameState::IsInPlayerTurn()
{
	return MainPlayerCamp == CurPlayer;
}

void ALudoGameState::TurnPlayer()
{
	ALudoProjectGameMode* CurGameMode = Cast<ALudoProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurGameMode)
	{
		CurPlayer++;
		if (CurPlayer > CurGameMode->MaxPlayerNum)
		{
			CurPlayer = 1;
		}
		if (CurPlayer == MainPlayerCamp) // 每次进入玩家轮 更新状态
		{
			bAIActing = false;
			UpdateState(EGameState::EDice);
		}
		else // 启动AI操作
		{
			bAIActing = true;
			if (InitMap.Contains(CurPlayer))
			{
				UpdateState(EGameState::EDice);
			}
			else
			{
				UpdateState(EGameState::EStart);
			}
		}
	}
}

bool ALudoGameState::PickPiece(APieceCharacter* Piece)
{
	if (!Piece)
	{
		return false;
	}
	if (CurState == EGameState::EStart) // 首次提子 确定玩家正营 并且直接进行下一步 无需骰子
	{
		if (MainPlayerCamp == 0)
		{
			MainPlayerCamp = Piece->camp;
		}
		CurPlayer = Piece->camp;
		InitMap.Emplace(CurPlayer, true);
		CurPiece = Piece;
		GoPiece();
		return true;
	}
	if (CurState == EGameState::EWaitChoice && Piece->camp == CurPlayer)
	{
		if (Piece->state != EPieceState::EFlying && Piece->state != EPieceState::EArrived)
		{
			if (Piece->state == EPieceState::EPark)
			{
				if (CurDiceNum != 6)
				{
					if (GetUsefulPieces().Num() == 0)
					{
						// 如果没有可用棋子且并非6点 那么轮空
						TurnPlayer();
						return true;
					}
					return false;
				}
			}
			CurPiece = Piece;
			GoPiece();
			return true;
		}
	}
	return false;
}

bool ALudoGameState::GoPiece()
{
	// 记录当前棋子位置与状态
	if (CurPlayer == MainPlayerCamp)
	{
		SaveState();
	}
	UpdateState(EGameState::EBusy);
	if (CurPiece)
	{
		if (CurPiece->state == EPieceState::EPark)
		{
			CurPiece->GoSteps(1);
		}
		else
		{
			CurPiece->GoSteps(CurDiceNum);
		}
	}
	return true;
}

void ALudoGameState::UpdateState(EGameState NewState)
{
	CurState = NewState;
	OnStateUpdate.Broadcast();
}

void ALudoGameState::SetDice(uint8 step)
{
	CurDiceNum = step;
	UpdateState(EGameState::EWaitChoice);
}

void ALudoGameState::OnFlyDone()
{
	if (!CurPiece)
	{
		return;
	}
	// 检查撞子
	TArray<APieceCharacter*> OutPieces;
	ALudoProjectGameMode* CurGameMode = Cast<ALudoProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	for (auto&Piece : Pieces)
	{
		if (Piece != CurPiece && Piece->camp != CurPiece->camp && Piece->x == CurPiece->x && Piece->y == CurPiece->y)
		{
			OutPieces.Add(Piece);
		}
	}
	for (uint8 DelIndex = 0; DelIndex < OutPieces.Num();)
	{
		TArray<APieceCharacter*> Temp;
		for (auto& Park : CurGameMode->Parkings[OutPieces[DelIndex]->camp])
		{
			if (!GetPiece(Park->x, Park->y, Temp))
			{
				OutPieces[DelIndex]->HardReset(Park->x, Park->y, EPieceState::EPark);
				DelIndex++;
				break;
			}
		}
	}
	//检查到终点
	if (CurGameMode->GetRoute(CurPiece->x, CurPiece->y)->Type == ERouteType::EFinal)
	{
		TArray<APieceCharacter*> Temp;
		if (GetPiece(CurPiece->x, CurPiece->y, Temp))
		{
			// 检查胜利
			if (Temp.Num() == 4)
			{
				if (CurPiece->camp == MainPlayerCamp)
				{
					UpdateState(EGameState::EWin);
				}
				else
				{
					UpdateState(EGameState::ELose);
				}
				return;
			}
		}
	}

	if (CurDiceNum == 6)
	{
		// 继续骰子
		UpdateState(EGameState::EDice);
	}
	else
	{
		TurnPlayer();
	}
}



void ALudoGameState::OnLose()
{
	for (int32 i = 0; i < Pieces.Num(); i++)
	{
		Pieces[i]->HardReset(Pieces[i]->x, Pieces[i]->y, Pieces[i]->state == EPieceState::EFlying ? EPieceState::EActive : Pieces[i]->state, false);
	}
	UpdateState(EGameState::ELose);
}

TArray<APieceCharacter*> ALudoGameState::GetParkingPieces()
{
	TArray<APieceCharacter*> OutArray;
	for (auto&Piece: Pieces)
	{
		if (Piece->camp == CurPlayer && Piece->state == EPieceState::EPark)
		{
			OutArray.Add(Piece);
		}
	}
	return OutArray;
}

TArray<APieceCharacter*> ALudoGameState::GetUsefulPieces()
{
	TArray<APieceCharacter*> OutArray;
	for (auto&Piece : Pieces)
	{
		if (Piece->camp == CurPlayer && Piece->state == EPieceState::EActive)
		{
			OutArray.Add(Piece);
		}
	}
	return OutArray;
}