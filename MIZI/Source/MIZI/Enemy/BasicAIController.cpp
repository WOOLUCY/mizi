// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BasicAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Data/EnemyData.h"

ABasicAIController::ABasicAIController()
{
	// AIPerceptionComponent 초기화
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// 시야 감각 설정 초기화
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTree"));
}

void ABasicAIController::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicAIController::OnTargetPerceptionUpdated);

	if(!EnemyData)
	{
		ensure(EnemyData);
		RunBehaviorTree(BehaviorTree);

		// 시야 감각 설정 값 조정 (시야 반경, 시야각 등)
		SightConfig->SightRadius = 5000.0f;;
		SightConfig->LoseSightRadius = 5500.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		// AIPerceptionComponent에 시야 감각 추가
		AIPerceptionComponent->ConfigureSense(*SightConfig);

		// AIPerceptionComponent의 DominantSense 설정(우선순위가 되는 감각)
		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
	}
	else
	{
		RunBehaviorTree(EnemyData->DefaultBehaviorTree);
	}
}

void ABasicAIController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
}

void ABasicAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetData(DataTableRowHandle);

}

void ABasicAIController::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FEnemyTableRow* Data = DataTableRowHandle.GetRow<FEnemyTableRow>(TEXT("BasicEnemy"));
	if (!Data) { ensure(false); return; }
	EnemyData = Data;


}

void ABasicAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	if(UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass())
	{
		if(Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), Actor);
			GetBlackboardComponent()->SetValueAsVector(FName("LastKnownLocation"), Stimulus.StimulusLocation);
		}
		else
		{
			GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"),nullptr);
		}
	}
	else
	{
		GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), nullptr);
	}
	
}
