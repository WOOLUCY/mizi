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

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_45ap.SM_Shell_45ap'"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	}
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnOverlapBegin);
	StaticMeshComponent->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->InitialSpeed = 8000.f;
	ProjectileMovement->MaxSpeed = 8000.f;
	ProjectileMovement->ProjectileGravityScale = 0.05f;
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

void ABulletBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s is collided"), *GetName());
	// TODO: Spawn Emitter

	// TODO: Apply Damage

	Destroy();
}

//void ABulletBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	//ACharacter* Character = Cast<ACharacter>(OtherActor);
//	//if (!Character) return;
//
//	//float BaseDamage = 5.0f;
//	//TSubclassOf<UDamageType> DamageTypeClass;
//	//UGameplayStatics::ApplyDamage(Character, BaseDamage, nullptr, GetOwner(), DamageTypeClass);
//
//	//// Spawn Emitter
//	//if (ParticleEffect)
//	//{
//	//	FVector Location = GetActorLocation();
//	//	FRotator Rotation = FRotator(0.0, 90.0, 0.0);
//	//	FVector Scale = FVector(0.3, 0.3, 0.3);
//
//	//	// 파티클 생성
//	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect, Location, Rotation, Scale, true);
//	//}
//
//	// TODO: Play Sound When Play are attacked
//
//	
//}

