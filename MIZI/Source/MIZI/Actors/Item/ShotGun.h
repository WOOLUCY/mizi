// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/Rifle.h"
#include "ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API AShotGun : public ARifle
{
	GENERATED_BODY()

public:
	AShotGun();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

	virtual void OnUsed() override;

protected:
	virtual void FireBullet() override;

	UFUNCTION()
	void SetFireState();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

protected:
	bool bIsFiring = false;
};
