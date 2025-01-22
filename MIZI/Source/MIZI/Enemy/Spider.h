// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Data/EnemyData.h"

#include "Spider.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ASpider : public AEnemyBase
{
	GENERATED_BODY()
protected:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

public:	// Interface
	virtual float Attack() override;
};
