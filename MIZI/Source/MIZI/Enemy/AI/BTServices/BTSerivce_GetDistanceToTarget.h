// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTSerivce_GetDistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UBTSerivce_GetDistanceToTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTSerivce_GetDistanceToTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 블루프린트와 인스턴스 모두에서 편집 가능한 변수
	UPROPERTY(EditAnywhere, Category = "Settings")
	float MeleeAttackDistance = 250.0f;
};
