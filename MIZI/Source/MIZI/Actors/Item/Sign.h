// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Sign.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ASign : public AItemBase
{
	GENERATED_BODY()
public:
	ASign();
	virtual void OnUsed() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Sign")
	USoundBase* SwingSound;

	UPROPERTY(EditAnywhere, Category = "Sign")
	USphereComponent* HitSphere;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EmptyOverlappedActors();

public:
	TSet<AActor*> OverlappedActors;

private:
	bool bIsUsed = false;

};
