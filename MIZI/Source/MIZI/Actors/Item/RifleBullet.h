// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "RifleBullet.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ARifleBullet : public AItemBase
{
	GENERATED_BODY()

public:
	ARifleBullet();
	virtual void OnUsed() override;

private:
	UFUNCTION()
	void ReloadBullets();

protected:
	UPROPERTY(EditAnywhere)
	int32 BulletAmount = 10;

};
