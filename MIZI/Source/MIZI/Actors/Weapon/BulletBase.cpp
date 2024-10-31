// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/BulletBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Bullet->SetupAttachment(RootComponent);

	//Shell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//Shell->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Bullet);

	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABulletBase::OnEndOverlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->ProjectileGravityScale = 0.1f;

	// 빔 파티클 시스템을 로드
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Blueprint/Gimmick/Turret/PS_LaserHit.PS_LaserHit'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleEffect = ParticleAsset.Object;
	}
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character) return;

	float BaseDamage = 5.0f;
	TSubclassOf<UDamageType> DamageTypeClass;
	UGameplayStatics::ApplyDamage(Character, BaseDamage, nullptr, GetOwner(), DamageTypeClass);

	// Spawn Emitter
	if (ParticleEffect)
	{
		FVector Location = GetActorLocation();
		FRotator Rotation = FRotator(0.0, 90.0, 0.0);
		FVector Scale = FVector(0.3, 0.3, 0.3);

		// 파티클 생성
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect, Location, Rotation, Scale, true);
	}

	// TODO: Play Sound When Play are attacked

	Destroy();
}

