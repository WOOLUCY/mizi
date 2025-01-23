// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTasks/BTTask_MoveToSteppedLocation.h"
#include "AIController.h"
#include "Enemy/Spider.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h" // FPathFollowingResult, GetPathFollowingComponent()
#include "NavigationSystem.h"                 // UNavigationSystemV1 (Optional, 필요 시)

UBTTask_MoveToSteppedLocation::UBTTask_MoveToSteppedLocation()
{
	NodeName = "MoveToSteppedLocation";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToSteppedLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController->GetPawn();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (AIController && BlackboardComp && ControlledPawn)
    {
        // 이동 속도를 600으로 설정합니다.
        UCharacterMovementComponent* MovementComp = ControlledPawn->FindComponentByClass<UCharacterMovementComponent>();
        if (MovementComp)
        {
            MovementComp->MaxWalkSpeed = 600.0f;
        }

        // 목표 위치로 이동합니다.
        FVector TargetLocation = BlackboardComp->GetValueAsVector("SteppedLocation");
        AIController->MoveToLocation(TargetLocation);

        return EBTNodeResult::InProgress; // 이동 중일 때는 InProgress 반환
    }
    return EBTNodeResult::Failed;
}

void UBTTask_MoveToSteppedLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController->GetPawn();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (AIController && BlackboardComp && ControlledPawn)
    {
        FVector TargetLocation = BlackboardComp->GetValueAsVector("SteppedLocation");

        if (HasReachedTarget(AIController, TargetLocation))
        {
            // 이동 속도를 400으로 설정합니다.
            UCharacterMovementComponent* MovementComp = ControlledPawn->FindComponentByClass<UCharacterMovementComponent>();
            if (MovementComp)
            {
                MovementComp->MaxWalkSpeed = 400.0f;
            }

            // IsStepped 변수를 false로 설정합니다.
            BlackboardComp->SetValueAsBool("IsStepped", false);

            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); // 도달 시 Succeeded 반환
        }
    }
}

bool UBTTask_MoveToSteppedLocation::HasReachedTarget(AAIController* AIController, FVector TargetLocation)
{
    APawn* ControlledPawn = AIController->GetPawn();
    if (ControlledPawn)
    {
        FVector CurrentLocation = ControlledPawn->GetActorLocation();
        float Distance = FVector::Dist(CurrentLocation, TargetLocation);

        //UE_LOG(LogTemp, Warning, TEXT("Current Location: %s"), *CurrentLocation.ToString());
        //UE_LOG(LogTemp, Warning, TEXT("Target Location: %s"), *TargetLocation.ToString());
        //UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);

        return Distance <= DistTolerance;
    }
    return false;
}
