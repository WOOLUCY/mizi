// Fill out your copyright notice in the Description page of Project Settings.


#include "Flash.h"

#include "Kismet/GameplayStatics.h"

AFlash::AFlash()
{
	ItemName = TEXT("Flash");

	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	FlashLight->SetupAttachment(StaticMeshComponent);

	FlashLight->SetRelativeLocation(FVector(14.0, 0.0, 1.0));
	FlashLight->Intensity = 9000.0f;
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

	if (LightMaterial)
	{
		FlashLight->LightFunctionMaterial = LightMaterial;
	}

	if(!bIsTurnedOn)	// 플래쉬가 꺼져있을때
	{
		TurnOn();
	}
	else
	{
		TurnOff();
	}

	Super::OnUsed();
}

void AFlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsTurnedOn)
	{
		float CurrentOnTime = GetWorld()->GetTimeSeconds() - TurnOnTime + TotalOnTime;

		// 최대 사용 시간을 초과하면 전원 끄기 및 방전 처리
		if(CurrentOnTime >= DurationLimit)
		{
			TurnOff();
			bIsFlashDead = true;
		}
	}

}

void AFlash::TurnOn()
{
	if (bIsFlashDead) return;
	FlashLight->SetVisibility(true);
	bIsTurnedOn = true;

	// 켠 시간을 기록
	TurnOnTime = GetWorld()->GetTimeSeconds();
}

void AFlash::TurnOff()
{
	FlashLight->SetVisibility(false);
	bIsTurnedOn = false;

	// 켜져 있던 시간을 누적시킴
	TotalOnTime += GetWorld()->GetTimeSeconds() - TurnOnTime;

	UE_LOG(LogTemp, Warning, TEXT("The Flash is used for %f"), TotalOnTime);
}
