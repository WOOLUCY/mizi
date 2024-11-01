// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Components/SpotLightComponent.h"
#include "Flash.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API AFlash : public AItemBase
{
	GENERATED_BODY()

public:
	AFlash();
	virtual void OnUsed() override;

protected:
	UPROPERTY(EditAnywhere)
	USpotLightComponent* FlashLight;

	UPROPERTY(EditAnywhere)
	USoundBase* SwitchSound;

private:
	bool bIsTurnedOn = false;

};
