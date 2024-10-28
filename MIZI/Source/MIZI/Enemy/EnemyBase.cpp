// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

#include "BasicAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Data/EnemyData.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCanEverAffectNavigation(false);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	SetData(DataTableRowHandle);
}

void AEnemyBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
}

void AEnemyBase::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if(DataTableRowHandle.IsNull()) {return;}
	FEnemyTableRow* Data = DataTableRowHandle.GetRow<FEnemyTableRow>(TEXT("BasicEnemy"));
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
	if(EnemyData->SkeletalMesh)
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
	else if(EnemyData->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(EnemyData->StaticMesh);
		StaticMeshComponent->SetRelativeTransform(EnemyData->MeshTransform);

		StaticMeshComponent->SetVisibility(true);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		GetMesh()->SetVisibility(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	{
		AIControllerClass = ABasicAIController::StaticClass();
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemyBase::Attack()
{
	GetMovementComponent()->StopMovementImmediately();

	TArray<UAnimMontage*> AttackMontages = EnemyData->AttackMontage;
	if(AttackMontages.IsEmpty())
	{
		ensure(false);
		return 0.f;
	}

	TSubclassOf<UDamageType> DamageType;

	float AnimationDuration = PlayAnimMontage(AttackMontages[0]);
	float Damage = UGameplayStatics::ApplyDamage(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), EnemyData->Damage, nullptr, this, DamageType);
	UE_LOG(LogTemp, Log, TEXT("%f damaged"), Damage);
	return AnimationDuration;
}

