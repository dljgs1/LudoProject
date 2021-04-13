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
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh0"));
	HeadMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	HeadMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
}

void APieceCharacter::MoveToRoute(ULudoRoute* Target)
{
	Actor* Ptr = Target->Block;

}

void APieceCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}

