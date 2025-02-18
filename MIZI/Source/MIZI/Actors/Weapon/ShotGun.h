// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/Rifle.h"
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
protected:
	virtual void Fire() override;
};
