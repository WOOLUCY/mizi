// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/AIController/MaskAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AMaskAIController::AMaskAIController()
{
	SightConfig->SightRadius = 3000.f;
	SightConfig->LoseSightRadius = 3500.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);

	AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());

	// WARNING: 75% loading
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/Enemy/Mask/AI/BT_Mask.BT_Mask'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Script/AIModule.BlackboardData'/Game/Blueprint/Enemy/BB_WeepingAngel.BB_WeepingAngel'"));
	if (BB.Succeeded())
	{
		BlackboardData = BB.Object;
	}
}
