// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTTAsk_RandomTeleport.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UBTTAsk_RandomTeleport : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTAsk_RandomTeleport();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void FinishTask(UBehaviorTreeComponent& OwnerComp, bool bSuccess);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> TeleportSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeleportProbability = 10;
};
