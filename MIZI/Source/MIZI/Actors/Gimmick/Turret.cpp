// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gimmick/Turret.h"

#include "NavigationSystemTypes.h"
#include "DSP/MidiNoteQuantizer.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnitConversion.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerCharacter/BasicCharacter.h"
#include "Preferences/CascadeOptions.h"
#include "UI/ScannedEnemyWidget.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensing->SightRadius = 2000.f;
	PawnSensing->SensingInterval = 0.05f;
	PawnSensing->SetPeripheralVisionAngle(30.f);
	PawnSensing->OnSeePawn.AddDynamic(this, &ATurret::OnPawnSeen);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovement->RotationRate = FRotator(0.0, 10.0, 0.0);

	// 파티클 컴포넌트 생성 및 초기화
	Laser = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BeamParticleComponent"));
	Laser->SetupAttachment(RootComponent);

	// 빔 파티클 시스템을 로드
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Blueprint/Gimmick/Turret/PS_Laser.PS_Laser'"));
	if (ParticleAsset.Succeeded())
	{
		BeamParticleSystem = ParticleAsset.Object;
		Laser->SetTemplate(BeamParticleSystem);
	}


	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	//static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gimmick/WBP_ScannedGimmick.WBP_ScannedGimmick_C'"));
	//if (WidgetClass.Succeeded())
	//{
	//	Widget->SetWidgetClass(WidgetClass.Class);
	//}

	Widget->SetupAttachment(StaticMeshComponent);
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetPivot(FVector2D(0.3, 0.3));

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	Laser->DeactivateSystem();
	Laser->SetVisibility(false);

	InitialRotation = GetActorRotation();

	if(BeamParticleSystem)
	{
		// 파티클 시스템 활성화
		FVector SocketLocation = StaticMeshComponent->GetSocketLocation(TEXT("BulletSocket"));

		Laser->SetBeamSourcePoint(0, SocketLocation, 0);
		Laser->SetBeamEndPoint(0, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
	}

	if (TimeLineCurve)
	{
		FOnTimelineFloat ProgressUpdate;
		ProgressUpdate.BindUFunction(this, FName("OnLoseSightTimeline"));

		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindUFunction(this, FName("OnEndLoseSightTimeline"));


		TimelineComponent->AddInterpFloat(TimeLineCurve, ProgressUpdate);
		TimelineComponent->SetTimelineFinishedFunc(TimelineFinishedEvent);
	}

	// Widget
	if (Widget)
	{
		Widget->SetWidgetClass(WidgetClass);
		UScannedEnemyWidget* ScanWidget = Cast<UScannedEnemyWidget>(Widget->GetWidget());
		ScanWidget->EnemyName->SetText(FText::FromString(TEXT("Turret")));
		Widget->SetVisibility(false);
	}


	MaxTargetRotationAngle = UKismetMathLibrary::RandomFloatInRange(30.0, 180.0);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Laser
	//Laser->SetBeamSourcePoint(0, StaticMeshComponent->GetSocketLocation(TEXT("BulletSocket")), 0);
	FVector SocketLocation = StaticMeshComponent->GetSocketLocation(TEXT("BulletSocket"));
	Laser->SetBeamSourcePoint(0, SocketLocation, 0);
	Laser->SetBeamEndPoint(0, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());

	// Rotating Movement
	float AngleDiff = GetActorRotation().Yaw - InitialRotation.Yaw;

	if(AngleDiff >= MaxTargetRotationAngle || AngleDiff <= MinTargetRotationAngle)
	{
		FRotator NewRotationRate = (RotatingMovement->RotationRate) * -1.f;
		RotatingMovement->RotationRate = NewRotationRate;
	}
}

void ATurret::OnScanned()
{
	if (!Widget) { return; }
	Widget->SetVisibility(true);
	UScannedEnemyWidget* ScanWidget = Cast<UScannedEnemyWidget>(Widget->GetWidget());
	ScanWidget->PlayAnimation(ScanWidget->ScanEffect);

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnScanTimer"), 3.f, false);
}

void ATurret::OnScanTimer()
{
	Widget->SetVisibility(false);
	UScannedEnemyWidget* ScanWidget = Cast<UScannedEnemyWidget>(Widget->GetWidget());
}

void ATurret::OnPawnSeen(APawn* SeenPawn)
{
	Laser->ActivateSystem();
	Laser->SetVisibility(true);

	ABasicCharacter* Character = Cast<ABasicCharacter>(SeenPawn);

	if (!Character) return;

	FVector TargetLocation = SeenPawn->GetActorLocation();
	TargetLocation.Z = GetActorLocation().Z;
	FRotator NewRotation =  UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	SetActorRotation(NewRotation);

	SpawnBullet();

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnLoseSightOfPawn"), 0.2f, false);
}

void ATurret::SpawnBullet()
{
	// Spawn Bullet
	FTransform SocketTransform = StaticMeshComponent->GetSocketTransform(TEXT("BulletSocket"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(TurretBulletClass, SocketTransform, SpawnParams);
	ABulletBase* SpawnedBullet = Cast<ABulletBase>(SpawnedActor);

	if (SpawnedBullet)
	{
		Bullets.Add(SpawnedBullet);
	}
}

void ATurret::OnLoseSightOfPawn()
{
	TimelineComponent->PlayFromStart();
}

void ATurret::OnLoseSightTimeline(float Rotation)
{
	Laser->DeactivateSystem();
	Laser->SetVisibility(false);
	double NewYaw = UKismetMathLibrary::Lerp(GetActorRotation().Yaw, InitialRotation.Yaw, Rotation);
	SetActorRotation(FRotator((0.0), NewYaw, (0.0)));
}

void ATurret::OnEndLoseSightTimeline()
{
	for(auto Bullet : Bullets)
	{
		if(Bullet)
			Bullet->Destroy();
	}

	Bullets.Empty();
}
