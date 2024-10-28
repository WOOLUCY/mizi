// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTServices/BTSerivce_GetDistanceToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"

UBTSerivce_GetDistanceToTarget::UBTSerivce_GetDistanceToTarget()
{
	
}

void UBTSerivce_GetDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

	if(Actor)
	{
		auto* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
		float Distance = ControlledPawn->GetDistanceTo(Actor);

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("InMeleeAttackRange"), (Distance <= MeleeAttackDistance));
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("InMeleeAttackRange"), false);
	}


}
