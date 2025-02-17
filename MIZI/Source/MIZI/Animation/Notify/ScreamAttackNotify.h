// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notify/AttackNotify.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundBase.h"

#include "ScreamAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UScreamAttackNotify : public UAttackNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UNiagaraSystem* ScreamEffect;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	int32 TotalTicks = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float DamageInterval = 0.3f;
};
