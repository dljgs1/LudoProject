// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameLogic/PieceCharacter.h"
#include "GameLogic/LudoGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "LudoProjectGameMode.h"

APieceCharacter::APieceCharacter()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PieceHead.PieceHead"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
			, GreenMaterial(TEXT("/Game/Puzzle/Meshes/GreenMaterial.GreenMaterial"))
			, RedMaterial(TEXT("/Game/Puzzle/Meshes/RedMaterial.RedMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	PlayerMaterials = {nullptr,
		ConstructorStatics.RedMaterial.Get(),
		ConstructorStatics.GreenMaterial.Get(),
		ConstructorStatics.BlueMaterial.Get(),
		ConstructorStatics.OrangeMaterial.Get()
	};
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh0"));
	HeadMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	HeadMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	HeadMesh->SetupAttachment(DummyRoot);

}

void APieceCharacter::Init(int32 _x, int32 _y, uint8 _camp)
{
	x = _x;
	y = _y;
	camp = _camp;
	state = EPieceState::EPark;
	if (_camp > 0 && _camp < PlayerMaterials.Num())
	{
		HeadMesh->SetMaterial(0, PlayerMaterials[_camp]);
	}
}

void APieceCharacter::OnClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}
void APieceCharacter::OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void APieceCharacter::HandleClicked()
{
	if (state != EPieceState::EActive && state != EPieceState::EPark)return;
	if (ALudoGameState* GameState = Cast<ALudoGameState>(GetWorld()->GetGameState()))
	{
		GameState->PickPiece(this);
	}
}

void APieceCharacter::MoveToRoute(ULudoRoute* Target)
{
	ALudoProjectBlock* Block = Target->Block;
	OriginVec = GetActorLocation();
	TargetVec = Block->GetActorLocation();
	TargetVec.Z = OriginVec.Z;
	if (ALudoGameState* GameState = Cast<ALudoGameState>(GetWorld()->GetGameState()))
	{
		TArray<APieceCharacter*> OutArray;
		if (GameState->GetPiece(x, y, OutArray))
		{
			TargetVec.X += (OutArray.Num()% 2 ? 20 : -20) * OutArray.Num();
		}
	}
	x = Target->x;
	y = Target->y;
	MoveProgress = 0.0f;
}

void APieceCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (state == EPieceState::EFlying || bForceFly)
	{
		MoveProgress += DeltaSeconds * FlySpeed;
		if (MoveProgress > 1.0f)
		{
			SetActorLocation(TargetVec);
			if (bForceFly)
			{
				bForceFly = false;
			}
			else
			{
				GoSteps(ResSteps - 1);
			}
		}
		else
		{
			SetActorLocation((TargetVec - OriginVec) * MoveProgress + OriginVec);
		}
	}

}

void APieceCharacter::GoSteps(uint8 StepNum)
{
	if (StepNum > 0)
	{
		ResSteps = StepNum;
		ALudoProjectGameMode* CurGameMode = Cast<ALudoProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		ULudoRoute* CurRoute = CurGameMode->GetRoute(x, y);
		if (!CurRoute)
		{
			return;
		}
		state = EPieceState::EFlying;
		// 选择下一个点
		uint8 DestRoute = camp + (int32)ERouteType::EFinalP1 -1;
		ULudoRoute* NextRoute = nullptr;
		for (uint8 i = 0;i<CurRoute->Next.Num();i++)
		{
			if ((int32)CurRoute->Next[i]->Type == DestRoute)
			{
				NextRoute = CurRoute->Next[i];
				break;
			}
			else if(CurRoute->Next[i]->Type == ERouteType::ERoute || CurRoute->Next[i]->Type == ERouteType::ETransFinal)
			{
				NextRoute = CurRoute->Next[i];
			}
		}
		if (NextRoute)
		{
			MoveToRoute(NextRoute);
		}
	}
	else
	{
		state = EPieceState::EActive;
		OnStepOver();
	}
}

void APieceCharacter::OnStepOver()
{
	if (ALudoGameState* GameState = Cast<ALudoGameState>(GetWorld()->GetGameState()))
	{
		GameState->OnFlyDone();
	}
}

void APieceCharacter::HardReset(int32 _x, int32 _y, EPieceState _state, bool withAnimate)
{
	x = _x;
	y = _y;
	state = _state;
	if (ALudoProjectGameMode* CurGameMode = Cast<ALudoProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (ULudoRoute* Route = CurGameMode->GetRoute(x, y))
		{
			if (withAnimate)
			{
				bForceFly = true;
				MoveToRoute(Route);
			}
			else
			{
				FVector Loc = Route->Block->GetActorLocation();
				Loc.Z = GetActorLocation().Z;
				SetActorLocation(Loc);
			}
		}
	}
}