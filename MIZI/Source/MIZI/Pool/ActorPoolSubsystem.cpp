// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/ActorPoolSubsystem.h"

void UActorPoolSubsystem::SpawnEffect(const FTransform& SpawnTransform, const FDataTableRowHandle& InDataTableRowHandle)
{
	AEffect* NewEffect = EffectPool.GetActorFromPool();
	NewEffect->SetData(InDataTableRowHandle);
	NewEffect->SetActorTransform(SpawnTransform);
	NewEffect->Play();
}

void UActorPoolSubsystem::SpawnProjectile(const FTransform& SpawnTransform, const FDataTableRowHandle& InDataTableRowHandle)
{
	AProjectile* NewProjectile = ProjectilePool.GetActorFromPool();
	NewProjectile->SetData(InDataTableRowHandle);
	NewProjectile->SetActorTransform(SpawnTransform);
	NewProjectile->Shoot();
}

void UActorPoolSubsystem::SpawnHitEffectWithDecal(const FTransform& SpawnTransform, const FDataTableRowHandle& InDataTableRowHandle)
{
	AEffectWithDecal* NewEffect = HitEffectDecalPool.GetActorFromPool();
	NewEffect->SetData(InDataTableRowHandle);
	NewEffect->SetActorTransform(SpawnTransform);
	NewEffect->Play();
}

void UActorPoolSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	EffectPool.InitializePool(&InWorld, 20);
	//ProjectilePool.InitializePool(&InWorld, 30);
	HitEffectDecalPool.InitializePool(&InWorld, 20);
}