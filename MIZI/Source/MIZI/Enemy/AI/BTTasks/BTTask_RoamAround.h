// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RoamAround.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UBTTask_RoamAround : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RoamAround();

protected:
	// Task 실행 시 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
