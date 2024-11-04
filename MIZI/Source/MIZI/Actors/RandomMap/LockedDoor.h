// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RandomMap/DoorBase.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "LockedDoor.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ALockedDoor : public ADoorBase
{
	GENERATED_BODY()

public:
	ALockedDoor();

public:
	void OnOpen();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBoxBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
	UTimelineComponent* OpenTimeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* OpenTimeCurveFloat;

private:
	UFUNCTION()
	void OnOpenProgress(float Value);

	UFUNCTION()
	void OnOpenFinished();

private:
	bool bIsLocked = true;
};