// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToSteppedLocation.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UBTTask_MoveToSteppedLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToSteppedLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool HasReachedTarget(AAIController* AIController, FVector TargetLocation);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistTolerance = 100.0f;
};
