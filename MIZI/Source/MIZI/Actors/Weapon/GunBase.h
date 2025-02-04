// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Engine/DataTable.h"
#include "Data/GunData.h"

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
	virtual void OnConstruction(const FTransform& Transform) override;


protected:
	virtual void BeginPlay() override;

	FGunTableRow* GetGunTableRow() const{ return GunTableRow; }

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.GunTableRow"))
	FDataTableRowHandle GunDataTableRowHandle;

	FGunTableRow* GunTableRow = nullptr;
};
