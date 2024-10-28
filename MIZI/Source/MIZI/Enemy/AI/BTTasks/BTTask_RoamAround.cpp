// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTasks/BTTask_RoamAround.h"

#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_RoamAround::UBTTask_RoamAround()
{
	NodeName = "RoamAround";
}

EBTNodeResult::Type UBTTask_RoamAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn) return EBTNodeResult::Failed;

	FVector PawnLocation = ControlledPawn->GetActorLocation();
	FVector NewPawnLocation;

	if(UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, PawnLocation, NewPawnLocation, 2000.0))
	{
		OwnerComp.GetAIOwner()->MoveToLocation(NewPawnLocation);
		return EBTNodeResult::Succeeded;
	};

	return EBTNodeResult::Failed;
}
