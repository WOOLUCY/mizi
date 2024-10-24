// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTasks/BTTask_ChasePlayer.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = "ChasePlayer";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->MoveToLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation());
	return EBTNodeResult::Succeeded;
}
