// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Mask.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API AMask : public AEnemyBase
{
	GENERATED_BODY()
public:
	AMask();
	
protected:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual float Attack() override;

protected:
	void FacePlayer();
	void SetEncounterNum();

	void Activate();

	UFUNCTION()
	void ResetDetection();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MaskMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> ActiveSound;

protected:
	bool bIsDetected = false;
	int EncounterNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EncounterTolerance = 2;
};

