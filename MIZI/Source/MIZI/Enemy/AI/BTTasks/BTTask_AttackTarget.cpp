// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTasks/BTTask_AttackTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/AI/Interface/EnemyInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
	NodeName = "AttackTarget";
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if(!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyBase* ControlledPawn = Cast<AEnemyBase>(AIController->GetPawn());
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}


	if(UKismetSystemLibrary::DoesImplementInterface(ControlledPawn, UEnemyInterface::StaticClass()))
	{
		IEnemyInterface* InterfaceObject = Cast<IEnemyInterface>(ControlledPawn);
		if(!InterfaceObject)
		{
			return EBTNodeResult::Failed;
		}
		
		float AnimationDuration = InterfaceObject->Attack();	// 인터페이스 함수 호출

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &OwnerComp]()
			{
				FinishTask(OwnerComp, true);

			}, AnimationDuration + 1.f, false);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;

}

void UBTTask_AttackTarget::FinishTask(UBehaviorTreeComponent& OwnerComp, bool bSuccess)
{
    FinishLatentTask(OwnerComp, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}
