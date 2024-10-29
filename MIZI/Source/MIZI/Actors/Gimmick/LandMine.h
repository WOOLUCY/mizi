// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Components/BoxComponent.h"
#include "LandMine.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ALandMine : public AItemBase
{
	GENERATED_BODY()

public:
	ALandMine();

public:
	virtual void OnScanned() override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	virtual void OnTimer();

protected:
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> NewWidgetClass;
};
