// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pool/PooledActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/DecalComponent.h"



#include "Projectile.generated.h"

/**
 * 
 */
USTRUCT()
struct MIZI_API FProjectileTableRow : public FTableRowBase
{
	GENERATED_BODY()
	FProjectileTableRow();

public:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Projectile|Particle")
	FTransform MeshTransform = FTransform::Identity;
};


UCLASS()
class MIZI_API AProjectile : public APooledActor
{
	GENERATED_BODY()
public:
	AProjectile();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	virtual void Shoot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	UProjectileMovementComponent* ProjectileMovement;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.ProjectileTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FProjectileTableRow* ProjectileData;
};
