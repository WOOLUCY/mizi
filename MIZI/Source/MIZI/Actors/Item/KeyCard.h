// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "KeyCard.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API AKeyCard : public AItemBase
{
	GENERATED_BODY()

public:
	virtual void OnUsed() override;
};
