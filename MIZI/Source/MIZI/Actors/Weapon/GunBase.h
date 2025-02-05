// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Engine/DataTable.h"
#include "Data/GunData.h"
#include "UI/ScannedItemWidget.h"

#include "GunBase.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API AGunBase : public AItemBase
{
	GENERATED_BODY()
	
public:
	AGunBase();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle) override;

	virtual void OnEquiped() override;
	virtual void OnUnEquiped() override;
	virtual void OnUsed() override;

public:
	int32 GetCurBulletAmount() const { return CurBulletAmount; }
	int32 GetMaxBulletAmount() const { return MaxBulletAmount; }

	void SetCurBulletAmount(const int32 InCurBulletAmount) { CurBulletAmount = InCurBulletAmount; }
	void SetMaxBulletAmount(const int32 InMaxBulletAMount) { MaxBulletAmount = InMaxBulletAMount; }

protected:
	virtual void Fire();
	
	UFUNCTION()
	virtual void SetCanFire();

protected:
	UPROPERTY(EditAnywhere, Category = "Rifle")
	int32 CurBulletAmount;
	UPROPERTY(EditAnywhere, Category = "Rifle")
	int32 MaxBulletAmount;

protected:
	bool bCanFire = true;


protected:
	FGunTableRow* GunTableRow = nullptr;
};
