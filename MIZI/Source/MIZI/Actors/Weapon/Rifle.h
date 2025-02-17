// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/GunBase.h"

#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ARifle : public AGunBase
{
	GENERATED_BODY()

public:
	ARifle();

protected:
	UFUNCTION()
	virtual void Fire() override;
};
