// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RandomMap/DoorBase.h"

// Sets default values
ADoorBase::ADoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	DoorFrame->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoorBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

