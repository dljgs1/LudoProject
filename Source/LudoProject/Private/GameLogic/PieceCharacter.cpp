// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameLogic/PieceCharacter.h"
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

	HeadMesh->OnClicked.AddDynamic(this, &APieceCharacter::HandleClicked);
	HeadMesh->OnInputTouchBegin.AddDynamic(this, &APieceCharacter::HandleClicked);
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

void APieceCharacter::HandleClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	if (state != EPieceState::EActive)return;
	GoSteps(4);
	return;
}

void APieceCharacter::MoveToRoute(ULudoRoute* Target)
{
	ALudoProjectBlock* Block = Target->Block;
	TargetVec = Block->GetActorLocation() - this->GetActorLocation();
	TargetVec.Z = 0.0f;
	MoveProgress = 0.0f;
}

void APieceCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (state == EPieceState::EFlying)
	{
		float rate = DeltaSeconds * FlySpeed;
		MoveProgress += rate;
		if (MoveProgress > 1.0f)
		{
			rate -= MoveProgress - 1.0f;
		}
		AddMovementInput(TargetVec, rate, true);
		if (MoveProgress >= 1.0f)
		{
			GoSteps(ResSteps - 1);
		}
	}

}

void APieceCharacter::GoSteps(uint8 StepNum)
{
	if (StepNum > 0)
	{
		state = EPieceState::EFlying;
		ResSteps = StepNum;
		ALudoProjectGameMode* CurGameMode = Cast<ALudoProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		ULudoRoute* CurRoute = CurGameMode->GetRoute(x, y);
		// 选择下一个点
		uint8 DestRoute = camp + (int32)ERouteType::EFinalP1;
		ULudoRoute* NextRoute = nullptr;
		for (uint8 i = 0;i<CurRoute->Next.Num();i++)
		{
			NextRoute = CurRoute->Next[i];
			if ((int32)CurRoute->Next[i]->Type == DestRoute)
			{
				break;
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
	}
}
