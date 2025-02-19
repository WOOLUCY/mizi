// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/WeepingAngel.h"

#include "WeepingAngelAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy/BasicAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AWeepingAngel::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FEnemyTableRow* Data = DataTableRowHandle.GetRow<FEnemyTableRow>(TEXT("Statue"));
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
		AIControllerClass = AWeepingAngelAIController::StaticClass();
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
}

void AWeepingAngel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABasicAIController* AIController = Cast<ABasicAIController>(GetController());
	if (!AIController)
	{
		ensure(false);
		return;
	}

	//if(StaticMeshComponent->UPrimitiveComponent::WasRecentlyRendered(0.05))
	//{
	//	auto BB = AIController->GetBlackBoardComponent();
	//	BB->SetValueAsBool(FName("IsDetected"), true);
	//}
	//else
	//{
	//	auto BB = AIController->GetBlackBoardComponent();
	//	BB->SetValueAsBool(FName("IsDetected"), false);
	//}

}

float AWeepingAngel::Attack()
{
	TSubclassOf<UDamageType> DamageType;

	float Damage = UGameplayStatics::ApplyDamage(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), EnemyData->Damage, nullptr, this, DamageType);
	UE_LOG(LogTemp, Log, TEXT("Weeping Angel is attacking: %f damaged"), Damage);
	return 0.2f;
}
