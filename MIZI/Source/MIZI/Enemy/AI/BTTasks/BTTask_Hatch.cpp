// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTasks/BTTask_Hatch.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "Enemy/Spider.h"

UBTTask_Hatch::UBTTask_Hatch()
{
	NodeName = "Hatch";
}

EBTNodeResult::Type UBTTask_Hatch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!UKismetMathLibrary::RandomIntegerInRange(0, Possibility))
	{
		auto* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
		if (nullptr == ControlledPawn) return EBTNodeResult::Failed;

		ASpider* Spider = Cast<ASpider>(ControlledPawn);
		if(!Spider) return EBTNodeResult::Failed;

		// Spawn Egg
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector NewLocation = FindHatchLocation(Spider);
		FTransform SpawnTransform(Spider->GetActorRotation(), NewLocation);
		SpawnParams.Instigator = Spider;
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ASpiderEgg::StaticClass(), SpawnTransform, SpawnParams);
	
		ASpiderEgg* Egg = Cast<ASpiderEgg>(SpawnedActor);
		if(!Egg) return EBTNodeResult::Failed;
		Spider->EggLocations.Add(Egg, false);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Succeeded;
}

FVector UBTTask_Hatch::FindHatchLocation(const APawn* Pawn)
{
	FVector StartLocation = Pawn->GetActorLocation();
	FVector EndLocation = Pawn->GetActorLocation() - FVector(0.0, 0.0, 200.0);
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Pawn);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	if (!bHit)
	{
		return FVector(0.0, 0.0, 0.0);
	}

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 10.f, 0, 1.0f);
	return HitResult.Location + FVector(0.0, 0.0, 10.0);
}
