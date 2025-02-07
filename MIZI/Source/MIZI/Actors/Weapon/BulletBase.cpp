// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/BulletBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pool/ActorPoolSubsystem.h"
#include "Actors/Weapon/GunBase.h"


// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitBoxExtent(FVector(5.0f, 5.0f, 5.0f));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECC_PhysicsBody);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABulletBase::OnBulletHit);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_45ap.SM_Shell_45ap'"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	}
	StaticMeshComponent->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->InitialSpeed = 8000.f;
	ProjectileMovement->MaxSpeed = 8000.f;
	ProjectileMovement->ProjectileGravityScale = 0.05f;

	SetLifeSpan(2.f);
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

void ABulletBase::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	UE_LOG(LogTemp, Warning, TEXT("💥 Bullet Hit: %s"), *OtherActor->GetName());

	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Red, false, 2.0f);

	AGunBase* Gun = Cast<AGunBase>(GetOwner());
	if (Gun)
	{
		// 이펙트 생성
		FTransform NewTransform;
		NewTransform.SetLocation(Hit.ImpactPoint);

		FRotator Rotation = UKismetMathLibrary::Conv_VectorToRotator(Hit.ImpactNormal);
		Rotation.Roll += FMath::RandRange(-180.0f, 180.0f);
		NewTransform.SetRotation(Rotation.Quaternion());

		int32 RandInt = UKismetMathLibrary::RandomIntegerInRange(1, 3);
		float RandFloat = RandInt / 100.0f;
		FVector Scale = UKismetMathLibrary::Conv_FloatToVector(RandFloat);
		NewTransform.SetScale3D(Scale);

		GetWorld()->GetSubsystem<UActorPoolSubsystem>()->SpawnHitEffectWithDecal(NewTransform, Gun->GetGunTableRow()->HitEffectTableRowHandle);
	
		// 데미지 적용
		float Damage = Gun->GetGunTableRow()->Damage;
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
	}

	// 총알 제거
	Destroy();
}

