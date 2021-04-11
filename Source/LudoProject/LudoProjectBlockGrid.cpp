// Copyright Epic Games, Inc. All Rights Reserved.

#include "LudoProjectBlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "HAL/PlatformFilemanager.h"
#include "FileHelpers.h"

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
	BlockSpacing = 300.f;
}


void ALudoProjectBlockGrid::BeginPlay()
{
	Super::BeginPlay();
	FString TxtStream;
	TArray<int32> NumArray;
	FString TxtPath = (FPaths::ProjectContentDir() + TEXT("MapConfig.txt"));
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*TxtPath))
	{
		FFileHelper::LoadFileToString(TxtStream, *TxtPath);
		TCHAR* TxtData = TxtStream.GetCharArray().GetData();
		for (int i = 0; i < TxtStream.Len(); i++)
		{
			if (TxtData[i] >= '0' && TxtData[i] <= '9' )
			{
				int32 Num = (int32)(TxtData[i] - '0');
				NumArray.Add(Num);
			}
		}
		Size = (int32)FMath::Sqrt(NumArray.Num());
	}
	else
	{
		NumArray = { 0,2,3,0,5,6,0,8,9 };
	}
	BlockSpacing = 900.0 / Size;
	// Number of blocks
	const int32 NumBlocks = Size * Size;

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		if (NumArray[BlockIndex] == 0)
		{
			continue;
		}
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
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
