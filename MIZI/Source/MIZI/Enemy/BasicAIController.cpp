// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BasicAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Data/EnemyData.h"
#include "Engine/CoreSettings.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/BasicCharacter.h"

ABasicAIController::ABasicAIController()
{
	/* AI Perception Setting */
	// AIPerceptionComponent 초기화
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// 시야 감각 설정 초기화
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// 시야 감각 설정 값 조정 (시야 반경, 시야각 등)
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// AIPerceptionComponent에 시야 감각 추가
	AIPerceptionComponent->ConfigureSense(*SightConfig);

	// AIPerceptionComponent의 DominantSense 설정(우선순위가 되는 감각)
	AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/Enemy/BehaivorTree/BT_Test.BT_Test'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Script/AIModule.BlackboardData'/Game/Blueprint/Enemy/BB_WeepingAngel.BB_WeepingAngel'"));
	if (BB.Succeeded())
	{
		//BlackboardComponent = BB.Object;
		BlackboardData = BB.Object;
	}

	// 블랙보드 컴포넌트 생성
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

}

void ABasicAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BlackboardComponent && BlackboardData)
	{
		// 블랙보드 컴포넌트에 블랙보드 데이터 초기화
		BlackboardComponent->InitializeBlackboard(*BlackboardData);
	}

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicAIController::OnTargetPerceptionUpdated);

	if (BehaviorTree != nullptr)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void ABasicAIController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ABasicAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABasicAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ABasicAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	if(UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass())
	{
		if(Stimulus.WasSuccessfullySensed())
		{
			if (Cast<ABasicCharacter>(Actor))	// 플레이어만 감지하도록
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
				BlackboardComponent->SetValueAsVector(FName("LastKnownLocation"), Stimulus.StimulusLocation);
			}
		}
		else
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"),nullptr);
		}
	}
	else
	{
		BlackboardComponent->SetValueAsObject(FName("TargetActor"), nullptr);
	}
	
}
