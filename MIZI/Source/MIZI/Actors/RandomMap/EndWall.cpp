// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RandomMap/EndWall.h"

AEndWall::AEndWall()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	//Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	//Wall->SetupAttachment(DefaultSceneRoot);
}
