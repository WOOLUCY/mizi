// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pool/PooledActor.h"
#include "Particles/ParticleSystemComponent.h"

#include "Effect.generated.h"

USTRUCT()
struct MIZI_API FEffectTableRow : public FTableRowBase
{
	GENERATED_BODY()
	FEffectTableRow();

public:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TSubclassOf<AEffect> EffectClass;

public:
	UPROPERTY(EditAnywhere, Category = "Effect|Sound")
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = "Effect|Sound")
	float VolumeMultiplier = 1.f;

public:
	UPROPERTY(EditAnywhere, Category = "Effect|Particle")
	TObjectPtr<UParticleSystem> Particle;

	UPROPERTY(EditAnywhere, Category = "Effect|Particle")
	FTransform ParticleTransform = FTransform::Identity;

public:
	UPROPERTY(EditAnywhere, Category = "Effect|Decal")
	FTransform DecalTransform;

	UPROPERTY(EditAnywhere, Category = "Effect|Decal")
	UMaterial* DecalMaterial;
};


UCLASS()
class MIZI_API AEffect : public APooledActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEffect();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

	virtual void Play();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PlaySound();
	virtual void PlayParticle();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSystemComponent = nullptr;

protected: // Sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Sound")
	TObjectPtr<USoundBase> Sound;

	// Volume Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Sound")
	float VolumeMultiplier = 1.f;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.EffectTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FEffectTableRow* EffectData;

};
