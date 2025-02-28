// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_HiddenPatrol.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UBTTask_HiddenPatrol : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_HiddenPatrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
