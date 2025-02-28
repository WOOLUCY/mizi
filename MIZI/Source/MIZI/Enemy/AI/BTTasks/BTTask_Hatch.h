// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Hatch.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UBTTask_Hatch : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Hatch();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	FVector FindHatchLocation(const APawn* Pawn);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Possibility = 1;
};
