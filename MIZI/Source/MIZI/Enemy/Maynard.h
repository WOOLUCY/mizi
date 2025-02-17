// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Maynard.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API AMaynard : public AEnemyBase
{
	GENERATED_BODY()

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void SetCanMove();
};
