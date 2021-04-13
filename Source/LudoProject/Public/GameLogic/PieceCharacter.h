// Copyright Epic Games, Inc. All Rights Reserved.
/*
棋子角色类
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameLogic/LudoRoute.h"
#include "PieceCharacter.generated.h"
/**  */

UCLASS()
class APieceCharacter : public ACharacter
{
GENERATED_BODY()
public:
	int32 x;
	int32 y;
	int32 direction;
	uint8 camp; // 阵营

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* HeadMesh;

	APieceCharacter();

	void MoveToRoute(ULudoRoute* Target);

	void Tick(float DeltaSeconds) override;
};
