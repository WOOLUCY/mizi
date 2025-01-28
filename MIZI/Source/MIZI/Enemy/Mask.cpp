// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Mask.h"

#include "AI/AIController/MaskAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AMask::AMask()
{
	MaskMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MaskMeshComponent"));
	MaskMesh->SetupAttachment(GetMesh(), TEXT("HeadBase"));
	MaskMesh->SetCanEverAffectNavigation(false);
}

void AMask::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FEnemyTableRow* Data = DataTableRowHandle.GetRow<FEnemyTableRow>(TEXT("Mask"));
	if (!Data) { ensure(false); return; }

	EnemyData = Data;
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		Movement->bOrientRotationToMovement = true;
		Movement->MaxWalkSpeed = EnemyData->WalkMaxSpeed;
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

		SkeletalMeshComponent->SetVisibility(false);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	if (EnemyData->StaticMesh)
	{
		MaskMesh->SetStaticMesh(EnemyData->StaticMesh);
		FTransform MaskTransform = FTransform(
			FRotator(0.0f, 180.0f, 90.0f), 
			FVector(-0.027018f, -0.551520f, 0.756281f), 
			FVector(0.011f, 0.011f, 0.011f) 
		);
		MaskMesh->SetRelativeTransform(MaskTransform);
	}
	{
		AIControllerClass = AMaskAIController::StaticClass();
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
}

void AMask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FacePlayer();
	SetEncounterNum();
}

float AMask::Attack()
{
	TSubclassOf<UDamageType> DamageType;

	float Damage = UGameplayStatics::ApplyDamage(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), EnemyData->Damage, nullptr, this, DamageType);
	UE_LOG(LogTemp, Log, TEXT("%f damaged"), Damage);
	return 0.0f;
}

void AMask::FacePlayer()
{
	FVector StartLocation = GetActorLocation();
	FVector TargetLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	SetActorRotation(NewRotation);
}

void AMask::SetEncounterNum()
{
	if (!MaskMesh->WasRecentlyRendered(0.05f) || bIsDetected) return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector PlayerLocation = Player->GetActorLocation();
	FVector MaskLocation = GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, MaskLocation);

	// Does player look forward mask?
	FVector PlayerForward = Player->GetActorForwardVector();
	FVector LookAtDirection = UKismetMathLibrary::Conv_RotatorToVector(LookAtRotation);
	float DotProduct = UKismetMathLibrary::Dot_VectorVector(LookAtDirection, PlayerForward);

	if (DotProduct < 0.5f) return;

	bIsDetected = true;
	EncounterNum++;
	UE_LOG(LogTemp, Warning, TEXT("Mask is detected: %d"), EncounterNum);

	if (EncounterNum > EncounterTolerance)
	{
		Activate();
		return;
	}

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("ResetDetection"), 4.0f, false);


}

void AMask::Activate()
{
	if (!bIsDetected) return;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return;
	if (Blackboard->GetValueAsBool(TEXT("IsActive"))) return;
	Blackboard->SetValueAsBool(TEXT("IsActive"), true);

	SetActorHiddenInGame(false);
	MaskMesh->SetVisibility(false);
	GetMesh()->SetVisibility(true);

	if (!ActiveSound) return;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ActiveSound, GetActorLocation());
}

void AMask::ResetDetection()
{
	bIsDetected = false;
}
