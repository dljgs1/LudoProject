// Copyright Epic Games, Inc. All Rights Reserved.

#include "LudoProjectBlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "HAL/PlatformFilemanager.h"
#include "FileHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "LudoProjectGameMode.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ALudoProjectBlockGrid::ALudoProjectBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
	Size = 3;
	MaxWidth = 900.f;
}


void ALudoProjectBlockGrid::BeginPlay()
{
	Super::BeginPlay();
	ALudoProjectGameMode* CurGameMode = Cast<ALudoProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!CurGameMode)
	{
		return;
	}
	Size = CurGameMode->MapWidth;
	float BlockSpacing = MaxWidth / Size;
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			int32 Number = CurGameMode->MapArray[x][y];
			if (Number == 0)
			{
				continue;
			}
			const float XOffset = x * BlockSpacing;
			const float YOffset = y * BlockSpacing;
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();
			ALudoProjectBlock* NewBlock;
			if (BPBlockClass.Get())
			{
				NewBlock = GetWorld()->SpawnActor<ALudoProjectBlock>(BPBlockClass, BlockLocation, FRotator(0, 0, 0));
			}
			else
			{
				NewBlock = GetWorld()->SpawnActor<ALudoProjectBlock>(BlockLocation, FRotator(0, 0, 0));
			}

			// Tell the block about its owner
			if (NewBlock != nullptr)
			{
				NewBlock->OwningGrid = this;
				NewBlock->SetBlockStyle(Number);
			}
		}
	}

}


void ALudoProjectBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}






#undef LOCTEXT_NAMESPACE
