// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "EndWall.generated.h"

/**
 * 
 */

UCLASS()
class MIZI_API AEndWall : public AActor
{
	GENERATED_BODY()
public:
	AEndWall();

public:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot = nullptr;

	//UPROPERTY(VisibleAnywhere)
	//UStaticMeshComponent* Wall;
};
