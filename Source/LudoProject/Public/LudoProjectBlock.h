// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LudoProjectBlock.generated.h"





/** A block that can be clicked */
UCLASS(minimalapi)
class ALudoProjectBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;
	int32 GridX;
	int32 GridY;
public:
	ALudoProjectBlock();

	/** Are we currently active? */
	bool bIsActive;

	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
		class UMaterialInstance* OrangeMaterial;
	UPROPERTY()
		class UMaterialInstance* GreenMaterial;
	UPROPERTY()
		class UMaterialInstance* RedMaterial;
	UPROPERTY()
		class UMaterialInstance* BlackMaterial;

	/** Grid that owns us */
	UPROPERTY()
	class ALudoProjectBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void HandleClicked();
	void SetGridPos(int32 x, int32 y);
	void Highlight(bool bOn);

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }


	/////////////////
	void SetBlockStyle(int32 type);

};



