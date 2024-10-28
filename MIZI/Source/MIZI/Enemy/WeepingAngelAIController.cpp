// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/WeepingAngelAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AWeepingAngelAIController::AWeepingAngelAIController()
{
	// 시야 감각 설정 값 조정 (시야 반경, 시야각 등)
	SightConfig->SightRadius = 5000.f;
	SightConfig->LoseSightRadius = 5500.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	SightConfig->SetMaxAge(7.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// AIPerceptionComponent에 시야 감각 추가
	AIPerceptionComponent->ConfigureSense(*SightConfig);

	// AIPerceptionComponent의 DominantSense 설정(우선순위가 되는 감각)
	AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/Enemy/Statue/BT_WeepingAngel.BT_WeepingAngel'"));
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
}
