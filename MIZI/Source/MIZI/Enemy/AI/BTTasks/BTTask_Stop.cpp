// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTasks/BTTask_Stop.h"

#include "AIController.h"

UBTTask_Stop::UBTTask_Stop()
{
	NodeName = "Stop";
}

EBTNodeResult::Type UBTTask_Stop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->StopMovement();

	return EBTNodeResult::Succeeded;
}
