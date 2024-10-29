// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/BulletBase.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/ScannedEnemyWidget.h"

#include "Turret.generated.h"

UCLASS()
class MIZI_API ATurret : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnScanned();

public:
	UFUNCTION()
	virtual void OnScanTimer();

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void SpawnBullet();

	UFUNCTION()
	void OnLoseSightOfPawn();

	// 타임라인의 플로트 트랙을 바인딩할 함수
	UFUNCTION()
	void OnLoseSightTimeline(float Rotation);

	// 타임라인의 끝 이벤트를 바인딩할 함수
	UFUNCTION()
	void OnEndLoseSightTimeline();

protected:
	UPROPERTY(EditAnywhere, Category = "Turret")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Turret")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Turret")
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, Category = "Turret")
	URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditAnywhere, Category = "Turret")
	UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, Category = "Turret")
	UParticleSystemComponent* Laser;

	UPROPERTY(EditAnywhere, Category = "Turret")
	TSubclassOf<ABulletBase> TurretBulletClass;

	UPROPERTY(EditAnywhere, Category = "Turret")
	TSubclassOf<UScannedEnemyWidget> WidgetClass;

	// 타임라인 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Turret")
	TObjectPtr<UTimelineComponent> TimelineComponent;

	// 플로트 커브를 저장할 변수
	UPROPERTY(EditAnywhere, Category = "Turret")
	TObjectPtr<UCurveFloat> TimeLineCurve;

private:
	UPROPERTY(EditAnywhere, Category = "Turret")
	UParticleSystem* BeamParticleSystem;

protected:
	UPROPERTY(EditAnywhere, Category = "Turret")
	float MaxTargetRotationAngle = 90.f;

	UPROPERTY(EditAnywhere, Category = "Turret")
	float MinTargetRotationAngle = 0.f;

protected:
	UPROPERTY(EditAnywhere, Category = "Turret")
	FString Name = TEXT("Turret");

private:
	FRotator InitialRotation;
	FTimeline TimeLine;

	TArray<ABulletBase*> Bullets;
};
