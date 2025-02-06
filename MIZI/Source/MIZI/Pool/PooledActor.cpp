// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/PooledActor.h"
#include "Pool/ActorPool.h"

// Sets default values
APooledActor::APooledActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APooledActor::ReturnToPool()
{
	OwningPool->ReturnActorToPool(this);
}

// Called when the game starts or when spawned
void APooledActor::BeginPlay()
{
	Super::BeginPlay();
	bTick = PrimaryActorTick.bCanEverTick;
}

void APooledActor::LifeSpanExpired()
{
	ReturnToPool();
}

// Called every frame
void APooledActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

