// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
//#include "Enemy/EnemyBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "BasicAIController.generated.h"

struct FEnemyTableRow;
class AEnemyBase;
/**
 * 
 */
UCLASS()
class MIZI_API ABasicAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABasicAIController();

protected:
	virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void OnPossess(APawn* InPawn) override;

protected:
    virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

	UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
    // AI 인식 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UAIPerceptionComponent* AIPerceptionComponent;

    // 시야 감각 설정
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UAISenseConfig_Sight* SightConfig;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBehaviorTree* BehaviorTree;


protected:
    UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.EnemyTableRow"))
    FDataTableRowHandle DataTableRowHandle;

    FEnemyTableRow* EnemyData;
};
