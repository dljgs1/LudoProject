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

UENUM()
enum class EPieceState : int32
{
	EPark = 0, // 未激活
	EActive = 1, // 激活中
	EFlying = 2, // 运动中
	EArrived = 3, // 抵达/胜利
};

UCLASS()
class APieceCharacter : public ACharacter
{
GENERATED_BODY()
public:
	int32 x;
	int32 y;
	int32 direction;
	uint8 camp; // 阵营
	EPieceState state = EPieceState::EPark;
	bool bPick = false;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Ludo, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* HeadMesh;
	UPROPERTY(Category = Ludo, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float PickHeight = 20.0f; // 棋子被拿起后的高度
	UPROPERTY(Category = Ludo, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray< UMaterialInstance*>  PlayerMaterials; //不同角色对应的材质
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* DummyRoot;
	APieceCharacter();
		
	void Init(int32 _x, int32 _y, uint8 _camp);

	void HandlePick(bool isPick);

	void MoveToRoute(ULudoRoute* Target);

	void Tick(float DeltaSeconds) override;
};
