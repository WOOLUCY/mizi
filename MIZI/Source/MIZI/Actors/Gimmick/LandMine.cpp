// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gimmick/LandMine.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

ALandMine::ALandMine()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(StaticMeshComponent);

	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALandMine::OnEndOverlap);
}

void ALandMine::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* OverlappedPawn = Cast<APawn>(OtherActor);

	if (!OverlappedPawn) return;

	// Radial Damage
	float BaseDamage = GimmickData->Damage;
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
