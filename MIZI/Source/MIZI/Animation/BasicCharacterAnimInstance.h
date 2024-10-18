// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasicCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UBasicCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	class UPawnMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly)
	double Speed = 0.0;

	UPROPERTY(BlueprintReadOnly)
	double Direction = 0.0;

	UPROPERTY(BlueprintReadOnly)
	bool bShoudMove = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsCrouch = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling = false;
};
