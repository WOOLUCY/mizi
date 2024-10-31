// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GimmickBase.h"
#include "Actors/Weapon/BulletBase.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Perception/PawnSensingComponent.h"


#include "Turret.generated.h"

UCLASS()
class MIZI_API ATurret : public AGimmickBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle) override;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

private:
	void RotateTurretRegularly();
	void SetLaserBeamPoint();

	UFUNCTION()
	void SpawnBullet();

	UFUNCTION()
	void OnLoseSightOfPawn();

protected:
	UPROPERTY(EditAnywhere, Category = "Turret")
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	URotatingMovementComponent* RotatingMovement;

	//UPROPERTY(EditAnywhere, Category = "Turret")
	//TSubclassOf<ABulletBase> TurretBulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UParticleSystemComponent* Laser;

	UPROPERTY(EditAnywhere, Category = "Turret")
	TSubclassOf<ABulletBase> TurretBulletClass;

protected:
	UPROPERTY(EditAnywhere, Category = "Turret")
	float MaxTargetRotationAngle = 90.f;

	UPROPERTY(EditAnywhere, Category = "Turret")
	float MinTargetRotationAngle = 0.f;

private:
	FRotator InitialRotation;

	//TArray<ABulletBase*> Bullets;
	bool bIsDetecting = false;

	TArray<AActor*> Bullets;

	FTimerHandle TimerHandle;
};
