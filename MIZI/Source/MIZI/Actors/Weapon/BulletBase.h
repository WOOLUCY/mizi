// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletBase.generated.h"

UCLASS()
class MIZI_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, Category = "Bullet")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	UStaticMeshComponent* Bullet;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	UStaticMeshComponent* Shell;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	UProjectileMovementComponent* ProjectileMovement;

private:
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ParticleEffect;
};
