// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gimmick/LandMine.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Particles/ParticleSystem.h"
#include "UI/ScannedEnemyWidget.h"

ALandMine::ALandMine()
{
	PrimaryActorTick.bCanEverTick = true;

	// NewWidgetClass도 미리 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> NewWidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gimmick/WBP_ScannedGimmick.WBP_ScannedGimmick_C'"));
	if (NewWidgetBPClass.Succeeded())
	{
		NewWidgetClass = NewWidgetBPClass.Class;
	}

	//static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT(""));
	//if (WidgetClass.Succeeded())
	//{
	//	Widget->SetWidgetClass(WidgetClass.Class);
	//}

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(StaticMeshComponent);

	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALandMine::OnEndOverlap);
}

void ALandMine::OnScanned()
{
	if (!Widget) { return; }
	Widget->SetVisibility(true);
	UScannedEnemyWidget* ScanWidget = Cast<UScannedEnemyWidget>(Widget->GetWidget());

	if (!ScanWidget) return;

	ScanWidget->PlayAnimation(ScanWidget->ScanEffect);

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnTimer"), 3.f, false);
}

void ALandMine::OnTimer()
{
	Widget->SetVisibility(false);
}

void ALandMine::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;

	UKismetSystemLibrary::GetComponentBounds(StaticMeshComponent, Origin, BoxExtent, SphereRadius);
	BoxCollision->SetBoxExtent(BoxExtent);
}

void ALandMine::BeginPlay()
{
	SetData(DataTableRowHandle);

	if(NewWidgetClass)
	{
		Widget->SetWidgetClass(NewWidgetClass);
		Widget->InitWidget();
	}

	UScannedEnemyWidget* EnemyWidget = Cast<UScannedEnemyWidget>(Widget->GetWidget());

	if (!EnemyWidget) return;
	EnemyWidget->DisplayName->SetText(ItemTableRow->DisplayName);

	if (!Widget) { return; }
	Widget->SetVisibility(false);
}

void ALandMine::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* OverlappedPawn = Cast<APawn>(OtherActor);

	if (!OverlappedPawn) return;

	// Radial Damage
	float BaseDamage = 100.f;
	float DamageRadius = 100.f;
	const TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage, GetActorLocation(), DamageRadius, nullptr, IgnoreActors);


	// Spawn Emitter
	UParticleSystem* ParticleTemplate = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ParticleTemplate)
	{
		FVector SpawnLocation = GetActorLocation(); 
		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnScale = FVector(8.0, 8.0, 8.0);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, SpawnLocation, SpawnRotation, SpawnScale, true);
	}

	// Play Sound
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'"));
	if (Sound)
	{
		FVector Location = GetActorLocation();  

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
	}

	Destroy();
}
