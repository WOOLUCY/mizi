// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Data/EnemyData.h"


#include "WeepingAngel.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API AWeepingAngel : public AEnemyBase
{
	GENERATED_BODY()

protected:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	// Interface
	virtual float Attack() override;
};
