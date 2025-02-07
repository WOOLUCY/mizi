// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/EffectWithDecal.h"
#include "Pool/ActorPoolSubsystem.h"
#include "Kismet/KismetMathLibrary.h"



AEffectWithDecal::AEffectWithDecal()
{
	HitDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Hit Decal"));
	HitDecalComponent->SetupAttachment(RootComponent);

	HitDecalComponent->SetFadeScreenSize(0.f);
}

void AEffectWithDecal::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	Super::SetData(InDataTableRowHandle);

	EffectWithDecalData = static_cast<FEffectWithDecalTableRow*>(EffectData);
	HitDecalComponent->SetMaterial(0, EffectWithDecalData->DecalMaterial);
	HitDecalComponent->SetRelativeTransform(EffectWithDecalData->DecalTransform);
}
