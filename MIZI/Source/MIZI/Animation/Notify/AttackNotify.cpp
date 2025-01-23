// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/Notify/AttackNotify.h"
#include "Enemy/EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* OwningActor = MeshComp->GetOwner();
	AEnemyBase* Enemy = Cast<AEnemyBase>(OwningActor);
	if (!Enemy) return;

	//UBlackboardComponent* BlackboardComp = Enemy->FindComponentByClass<UBlackboardComponent>();
	//UObject* Target = BlackboardComp->GetValueAsObject(FName("TargetActor"));
	//if (!Target) return;

	//AActor* TargetActor = Cast<AActor>(Target);
	//if (!TargetActor) return;

	FVector TargetLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	float Distance = FVector::Dist(Enemy->GetActorLocation(), TargetLocation);
	if (Distance <= AttackTolerance)
	{
		TSubclassOf<UDamageType> DamageType;
		float Damage = UGameplayStatics::ApplyDamage(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), Enemy->GetEnemyData()->Damage, nullptr, Enemy, DamageType);
		UE_LOG(LogTemp, Warning, TEXT("%f damaged"), Damage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("It's too far to attack."));
	}

	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
}
