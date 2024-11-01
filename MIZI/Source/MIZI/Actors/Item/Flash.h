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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditAnywhere, Category = "Flash")
	USpotLightComponent* FlashLight;

	UPROPERTY(EditAnywhere, Category = "Flash")
	USoundBase* SwitchSound;

	UPROPERTY(EditAnywhere, Category = "Flash")
	UMaterialInterface* LightMaterial;

	UPROPERTY(EditAnywhere, Category = "Flash")
	float DurationLimit = 10.f;

private:
	bool bIsTurnedOn = false;
	bool bIsFlashDead = false;

private:
	void TurnOn();
	void TurnOff();

private:
	float TurnOnTime;
	float TotalOnTime;

};
