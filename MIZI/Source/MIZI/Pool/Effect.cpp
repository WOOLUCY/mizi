// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/Effect.h"
#include "Kismet/GameplayStatics.h"
#include "Pool/ActorPoolSubsystem.h"

FEffectTableRow::FEffectTableRow()
	: EffectClass(AEffect::StaticClass())
{
}

// Sets default values
AEffect::AEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(DefaultSceneRoot);
	ParticleSystemComponent->bAutoActivate = false;
	RootComponent = DefaultSceneRoot;
}

void AEffect::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FEffectTableRow* Data = DataTableRowHandle.GetRow<FEffectTableRow>(TEXT("Effect"));
	if (!Data) { ensure(false); return; }

	EffectData = Data;

	Sound = Data->Sound;
	VolumeMultiplier = Data->VolumeMultiplier;

	ParticleSystemComponent->SetRelativeTransform(Data->ParticleTransform);
	ParticleSystemComponent->SetTemplate(Data->Particle);
	SetLifeSpan(0);
	SetLifeSpan(5.f);
}

void AEffect::Play()
{
	PlaySound();
	PlayParticle();
}

// Called when the game starts or when spawned
void AEffect::BeginPlay()
{
	Super::BeginPlay();

}

void AEffect::PlaySound()
{
	const FVector Location = GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location, VolumeMultiplier);
}

void AEffect::PlayParticle()
{
	ParticleSystemComponent->ActivateSystem(true);
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSTemplate, GetActorTransform());
}

// Called every frame
void AEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

