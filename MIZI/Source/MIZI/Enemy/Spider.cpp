// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Spider.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AIController/SpiderAIController.h"
#include "Components/CapsuleComponent.h"



void ASpider::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FEnemyTableRow* Data = DataTableRowHandle.GetRow<FEnemyTableRow>(TEXT("Spider"));
	if (!Data) { ensure(false); return; }

	EnemyData = Data;
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		//Movement->RotationRate = CharacterData->RotationRate;
		Movement->bOrientRotationToMovement = true;
		Movement->MaxWalkSpeed = EnemyData->WalkMaxSpeed;

		const float NewCapsuleHalfHeight = EnemyData->CollisionCapsuleHalfHeight * 0.5f;
		Movement->SetCrouchedHalfHeight(NewCapsuleHalfHeight);

		//Movement->bUseControllerDesiredRotation = true;
	}
	{
		UCapsuleComponent* Capsule = GetCapsuleComponent();
		if (!FMath::IsNearlyEqual(Capsule->GetUnscaledCapsuleHalfHeight(), EnemyData->CollisionCapsuleHalfHeight))
		{
			Capsule->SetCapsuleHalfHeight(EnemyData->CollisionCapsuleHalfHeight, false);
		}
	}
	if (EnemyData->SkeletalMesh)
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		SkeletalMeshComponent->SetSkeletalMesh(EnemyData->SkeletalMesh);
		SkeletalMeshComponent->SetRelativeTransform(EnemyData->MeshTransform);
		SkeletalMeshComponent->SetAnimClass(EnemyData->AnimClass);

		SkeletalMeshComponent->SetVisibility(true);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		StaticMeshComponent->SetVisibility(false);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else if (EnemyData->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(EnemyData->StaticMesh);
		StaticMeshComponent->SetRelativeTransform(EnemyData->MeshTransform);

		StaticMeshComponent->SetVisibility(true);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		GetMesh()->SetVisibility(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	{
		AIControllerClass = ASpiderAIController::StaticClass();
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
}

float ASpider::Attack()
{
	//GetMovementComponent()->StopMovementImmediately();

	TArray<UAnimMontage*> AttackMontages = EnemyData->AttackMontage;
	if (AttackMontages.IsEmpty())
	{
		ensure(false);
		return 0.f;
	}

	float AnimationDuration = PlayAnimMontage(AttackMontages[0]);
	return AnimationDuration;
}
