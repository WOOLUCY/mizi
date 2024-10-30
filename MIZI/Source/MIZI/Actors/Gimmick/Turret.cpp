// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gimmick/Turret.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerCharacter/BasicCharacter.h"


// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensing->SightRadius = 2000.f;
	PawnSensing->SensingInterval = 0.05f;
	PawnSensing->SetPeripheralVisionAngle(30.f);
	//PawnSensing->OnSeePawn.AddDynamic(this, &ATurret::OnPawnSeen);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovement->RotationRate = FRotator(0.0, 10.0, 0.0);

	Laser = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTurretRegularly();
	SetLaserBeamPoint();
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorRotation();

	UParticleSystem* ExplosionEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Blueprint/Gimmick/Turret/PS_Laser.PS_Laser'"));

	FVector Location = StaticMeshComponent->GetSocketLocation(TEXT("BulletSocket"));
	FRotator Rotation = FRotator::ZeroRotator;      

	if (ExplosionEffect)
	{
		Laser = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, Location, Rotation, true);
	}

	Laser->SetVisibility(false);

	if(PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ATurret::OnPawnSeen);
	}
}

void ATurret::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FGimmickTableRow* Data = DataTableRowHandle.GetRow<FGimmickTableRow>(TEXT("Turret"));
	if (!Data) { ensure(false); return; }

	GimmickData = Data;
	if (GimmickData->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(GimmickData->StaticMesh);
	}

	if (GimmickData->Damage)
	{
		Damage = GimmickData->Damage;
	}

	if (GimmickData->EnemyWidgetClass)
	{
		Widget->SetWidgetClass(GimmickData->EnemyWidgetClass);
	}
}

void ATurret::RotateTurretRegularly()
{
	if(bIsDetecting)
	{
		RotatingMovement->RotationRate.Yaw = 0.0;
	}
	else
	{
		RotatingMovement->RotationRate.Yaw = 10.0;

		float Diff = GetActorRotation().Yaw - InitialRotation.Yaw;
		if(Diff <= MinTargetRotationAngle || Diff >= MaxTargetRotationAngle)
		{
			RotatingMovement->RotationRate *= -1;
		}
	}
}

void ATurret::SetLaserBeamPoint()
{
	if (!Laser) return;

	FVector SourceLocation = StaticMeshComponent->GetSocketLocation(TEXT("BulletSocket"));
	Laser->SetBeamSourcePoint(0, SourceLocation, 0);

	FVector EndLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	Laser->SetBeamEndPoint(0, EndLocation);
}

void ATurret::SpawnBullet()
{
	// Spawn Bullet
	FTransform SocketTransform = StaticMeshComponent->GetSocketTransform(TEXT("BulletSocket"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if(!TurretBulletClass)
	{
		ensure(false);
		return;
	}
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(TurretBulletClass, SocketTransform, SpawnParams);
	ABulletBase* SpawnedBullet = Cast<ABulletBase>(SpawnedActor);

	if (SpawnedBullet)
	{
		Bullets.Add(SpawnedBullet);
	}

	// TODO: 소리 재생
	//UGameplayStatics::PlaySoundAtLocation
}

void ATurret::OnPawnSeen(APawn* SeenPawn)
{
	if (!Cast<ABasicCharacter>(SeenPawn)) return;

	bIsDetecting = true;

	Laser->SetVisibility(true);

	FVector Start = GetActorLocation();
	FVector End = SeenPawn->GetActorLocation();
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
	NewRotation.Roll = 0.0;
	NewRotation.Pitch = 0.0;
	SetActorRotation(NewRotation);

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("SpawnBullet"), 0.2f, false);
	//UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnLoseSightOfPawn"), 3.f, false);

	//FTimerDelegate TimerDelegate;
	//TimerDelegate.BindUFunction(this, FName("OnLoseSightOfPawn"));

	//GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.0f, false);
}

void ATurret::OnLoseSightOfPawn()
{
	Laser->SetVisibility(false);
	SetActorRotation(InitialRotation);
	bIsDetecting = false;
}
