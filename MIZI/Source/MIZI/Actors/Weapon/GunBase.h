// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Engine/DataTable.h"
#include "Data/GunData.h"
#include "UI/ScannedItemWidget.h"
#include "Pool/Effect.h"
#include "Pool/ActorPoolSubsystem.h"
#include "GameFramework/PawnMovementComponent.h"

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
	virtual void OnUsedTriggered() override;

public:
	int32 GetCurBulletAmount() const { return CurBulletAmount; }
	int32 GetMaxBulletAmount() const { return MaxBulletAmount; }

	void SetCurBulletAmount(const int32 InCurBulletAmount) { CurBulletAmount = InCurBulletAmount; }
	void SetMaxBulletAmount(const int32 InMaxBulletAMount) { MaxBulletAmount = InMaxBulletAMount; }

	const FGunTableRow* GetGunTableRow() const { return GunTableRow; }

protected:
	virtual void Fire();
	
	UFUNCTION()
	void SetCanFire();

protected:
	UPROPERTY(EditAnywhere, Category = "Rifle")
	int32 CurBulletAmount;
	UPROPERTY(EditAnywhere, Category = "Rifle")
	int32 MaxBulletAmount;

protected:
	bool bCanFire = true;

public:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.EffectTableRow"))
	FDataTableRowHandle EffectTableRowHandle;

protected:
	FGunTableRow* GunTableRow = nullptr;
};
