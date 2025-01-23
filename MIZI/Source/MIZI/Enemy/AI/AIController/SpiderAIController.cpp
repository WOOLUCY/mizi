// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/AIController/SpiderAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ASpiderAIController::ASpiderAIController()
{
	// 시야 감각 설정 값 조정 (시야 반경, 시야각 등)
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1500.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->SetMaxAge(1.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// AIPerceptionComponent에 시야 감각 추가
	AIPerceptionComponent->ConfigureSense(*SightConfig);

	// AIPerceptionComponent의 DominantSense 설정(우선순위가 되는 감각)
	AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());

	// WARNING: 75% loading
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/Enemy/Spider/AI/BT_Spider.BT_Spider'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Script/AIModule.BlackboardData'/Game/Enemy/Spider/AI/BB_Spider.BB_Spider'"));
	if (BB.Succeeded())
	{
		//BlackboardComponent = BB.Object;
		BlackboardData = BB.Object;
	}
}
