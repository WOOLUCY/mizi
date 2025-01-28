// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTasks/BTTAsk_RandomTeleport.h"

#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Misc/Utils.h"


UBTTAsk_RandomTeleport::UBTTAsk_RandomTeleport()
{
	NodeName = "RandomTeleport";
}

EBTNodeResult::Type UBTTAsk_RandomTeleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	int32 RandomChance = UKismetMathLibrary::RandomIntegerInRange(1, 100); // 1~100 사이의 랜덤 숫자

	if (RandomChance > TeleportProbability) return EBTNodeResult::Succeeded;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector PlayerLocation = Player->GetActorLocation();
	FVector PlayerForward = Player->GetActorForwardVector() * UKismetMathLibrary::RandomFloatInRange(-1000.f, -800.f);

	FVector ProjectedLocation;
	bool bProjectSuccess = UNavigationSystemV1::K2_ProjectPointToNavigation(
		GetWorld()
		, PlayerLocation + PlayerForward
		, ProjectedLocation
		, nullptr
		, nullptr
		, FVector(0.0f, 0.0f, 0.0f)
		);

	auto* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Succeeded;

	if (bProjectSuccess)
	{
		ControlledPawn->SetActorHiddenInGame(false);
		ControlledPawn->SetActorLocation(ProjectedLocation);

		// Play Teleport Sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld()
			, FUtils::GetRandomElementFromArray(TeleportSounds)
			, ControlledPawn->GetActorLocation());

		AAIController* AIController = OwnerComp.GetAIOwner();
		AIController->StopMovement();
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &OwnerComp]()
		{
			FinishTask(OwnerComp, true);

		}, 4.0f, false);

	return EBTNodeResult::InProgress;
}

void UBTTAsk_RandomTeleport::FinishTask(UBehaviorTreeComponent& OwnerComp, bool bSuccess)
{
	FinishLatentTask(OwnerComp, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}
