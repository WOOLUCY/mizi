// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"


#include "MasterRoom.generated.h"

UCLASS()
class MIZI_API AMasterRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnRandomEnemy();

public:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* OverlapFolder = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ExitsFolder = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* GeometryFolder = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SpawnFolder = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> EnemySpawnPoint;


protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> SpawnBox;
};
