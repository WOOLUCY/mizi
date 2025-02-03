// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ARifle : public AItemBase
{
	GENERATED_BODY()

public:
	ARifle();
	virtual void OnUsed() override;

	virtual void OnEquiped() override;
	virtual void OnUnEquiped() override;

public:
	int32 GetCurBulletAmount() const { return CurBulletAmount; }
	int32 GetMaxBulletAmount() const { return MaxBulletAmount; }

	void SetCurBulletAmount(const int32 InCurBulletAmount) {CurBulletAmount = InCurBulletAmount;}
	void SetMaxBulletAmount(const int32 InMaxBulletAMount) { MaxBulletAmount = InMaxBulletAMount;}

protected:
	UFUNCTION()
	virtual void FireBullet();

protected:
	UPROPERTY(EditAnywhere, Category = "Rifle")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Category = "Rifle")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	UPROPERTY(EditAnywhere, Category = "Rifle")
	UParticleSystem* FireEffect;

	UPROPERTY(EditAnywhere, Category = "Rifle")
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "Rifle")
	UMaterialInterface* BulletMarkDecal;


protected:
	UPROPERTY(EditAnywhere, Category = "Rifle")
	int32 CurBulletAmount = 10;
	UPROPERTY(EditAnywhere, Category = "Rifle")
	int32 MaxBulletAmount = 30;

	UPROPERTY(EditAnywhere, Category = "Rifle")
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Rifle")
	float Range = 3000.f;


};
