// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTasks/BTTask_HiddenPatrol.h"

#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_HiddenPatrol::UBTTask_HiddenPatrol()
{
	NodeName = "HiddenPatrol";
}

EBTNodeResult::Type UBTTask_HiddenPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn) return EBTNodeResult::Failed;

	ControlledPawn->SetActorHiddenInGame(true);

	FVector PawnLocation = ControlledPawn->GetActorLocation();
	FVector NewPawnLocation;

	if (UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, PawnLocation, NewPawnLocation, 2000.0f))
	{
		OwnerComp.GetAIOwner()->MoveToLocation(NewPawnLocation);
		return EBTNodeResult::Succeeded;
	};

	return EBTNodeResult::Failed;
}
