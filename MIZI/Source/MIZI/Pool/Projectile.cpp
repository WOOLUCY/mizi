// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pool/ActorPoolSubsystem.h"


FProjectileTableRow::FProjectileTableRow()
	: ProjectileClass(AProjectile::StaticClass())
{
}


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(TEXT("Projectile"));
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	//StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.f));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AProjectile::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FProjectileTableRow* Data = DataTableRowHandle.GetRow<FProjectileTableRow>(TEXT("SMG"));
	if (!Data) { ensure(false); return; }

	ProjectileData = Data;

	SetLifeSpan(1.f);

	StaticMeshComponent->SetStaticMesh(Data->StaticMesh);
	StaticMeshComponent->SetRelativeTransform(Data->MeshTransform);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectile::Shoot()
{
	ProjectileMovement->InitialSpeed = 10000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = 0.01f;
	ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;

	SetActorEnableCollision(true);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s is collided"), *GetName());
	if (!IsValid(this)) { return; }

	UE_LOG(LogTemp, Warning, TEXT("SweepResult.ImpactPoint: %s"), *SweepResult.ImpactPoint.ToString());
	UE_LOG(LogTemp, Warning, TEXT("SweepResult.ImpactNormal: %s"), *SweepResult.ImpactNormal.ToString());
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), OtherActor ? *OtherActor->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("bFromSweep: %s"), bFromSweep ? TEXT("TRUE") : TEXT("FALSE"));

	
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// TODO: Spawn Effect
	FTransform NewTransform;
	NewTransform.SetLocation(SweepResult.ImpactPoint);
	FRotator Rotation = UKismetMathLibrary::Conv_VectorToRotator(SweepResult.ImpactNormal);
	NewTransform.SetRotation(Rotation.Quaternion());
	GetWorld()->GetSubsystem<UActorPoolSubsystem>()->SpawnHitEffectWithDecal(NewTransform, ProjectileData->HitEffectTableRowHandle);


	// TODO: Apply Damage
}

