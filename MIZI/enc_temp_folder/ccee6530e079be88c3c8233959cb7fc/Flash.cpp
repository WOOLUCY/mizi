// Fill out your copyright notice in the Description page of Project Settings.


#include "Flash.h"

#include "Kismet/GameplayStatics.h"

AFlash::AFlash()
{
	ItemName = TEXT("Flash");

	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	FlashLight->SetupAttachment(StaticMeshComponent);

	FlashLight->SetRelativeLocation(FVector(14.0, 0.0, 1.0));
	FlashLight->Intensity = 6000.0f;
	FlashLight->LightColor = FColor(255, 252, 231, 255);
	FlashLight->AttenuationRadius = 3218.251953f;
	FlashLight->InnerConeAngle = 11.7f;
	FlashLight->OuterConeAngle = 30.0f;
	FlashLight->SourceRadius = 64.0f;
	FlashLight->SetVisibility(false);
}

void AFlash::OnUsed()
{
	if (SwitchSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SwitchSound);
	}

	if(!bIsTurnedOn)	// 플래쉬가 꺼져있을때
	{
		FlashLight->SetVisibility(true);
		bIsTurnedOn = true;
	}
	else
	{
		FlashLight->SetVisibility(false);
		bIsTurnedOn = false;
	}

	Super::OnUsed();
}
