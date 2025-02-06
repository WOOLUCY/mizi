// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActor.generated.h"

UCLASS()
class MIZI_API APooledActor : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	APooledActor();
	void SetOwnerPool(class FActorPool* NewOwnerPool) { OwningPool = NewOwnerPool; }
	void ReturnToPool();

	bool IsTickable() const { return bTick; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void LifeSpanExpired() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	class FActorPool* OwningPool;
	bool bTick = false;

};
