// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GimmickBase.h"
#include "Components/BoxComponent.h"
#include "LandMine.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ALandMine : public AGimmickBase
{
	GENERATED_BODY()

public:
	ALandMine();

protected:
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollision;
};
