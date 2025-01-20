// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Drone.generated.h"

UCLASS()
class MIZI_API ADrone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void OnFall();
	void OnArrive();
	void OnRise();
	void FindFloor();

	UFUNCTION()
	void SpawnItems();
	UFUNCTION()
	void ReSpawnItemsWithoutPhysics();
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void Activate();

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Drone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* ItemSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DroneMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimationAsset* BoxOpen;

	UPROPERTY(EditAnywhere)
	USoundBase* DroneSound;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 10.0f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<AItemBase>, int> OrderList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AItemBase*> SpawnedItems;

private:
	FVector StopLocation;
	bool bIsArrived = false;
	bool bIsRising = false;
	bool bIsActive = false;

};
