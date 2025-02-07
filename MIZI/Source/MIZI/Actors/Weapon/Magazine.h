// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Actors/Weapon/GunBase.h"
#include "Magazine.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API AMagazine : public AItemBase
{
	GENERATED_BODY()

public:
	AMagazine();
	virtual void OnUsed() override;

private:
	UFUNCTION()
	virtual void ReloadBullets();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGunBase> GunClass;

	UPROPERTY(EditAnywhere)
	USoundBase* ReloadSound = nullptr;	
};
