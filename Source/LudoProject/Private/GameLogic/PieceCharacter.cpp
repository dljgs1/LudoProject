// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameLogic/PieceCharacter.h"
#include "GameFramework/Actor.h"

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

void APieceCharacter::HandlePick(bool isPick)
{
	if (isPick == bPick)return;
	if (isPick)
	{
		SetActorLocation(GetActorLocation() + FVector(0.0, 0.0, PickHeight));
		bPick = true;
	}
	else
	{
		SetActorLocation(GetActorLocation() - FVector(0.0, 0.0, PickHeight));
		bPick = false;
	}
}

void APieceCharacter::MoveToRoute(ULudoRoute* Target)
{
	ALudoProjectBlock* Block = Target->Block;
	FVector TargetVec = Block->GetActorLocation() - this->GetActorLocation();
	TargetVec.Z = 0.0f;
	AddMovementInput(TargetVec, 1.0, true);
}

void APieceCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}