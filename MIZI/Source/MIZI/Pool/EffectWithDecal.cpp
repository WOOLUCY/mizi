// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/EffectWithDecal.h"
#include "Pool/ActorPoolSubsystem.h"
#include "Kismet/KismetMathLibrary.h"



AEffectWithDecal::AEffectWithDecal()
{
	HitDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Hit Decal"));
	HitDecalComponent->SetupAttachment(RootComponent);

	int32 RandInt = UKismetMathLibrary::RandomIntegerInRange(3, 7);
	float RandFloat = RandInt / 90.0f;

	FVector Scale = UKismetMathLibrary::Conv_FloatToVector(RandFloat);
	HitDecalComponent->SetWorldScale3D(Scale);
	HitDecalComponent->SetFadeScreenSize(0.f);	// 멀리 가면 데칼이 화면에 보이지 않는 것을 방지
}

void AEffectWithDecal::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	Super::SetData(InDataTableRowHandle);

	EffectWithDecalData = static_cast<FEffectWithDecalTableRow*>(EffectData);
	HitDecalComponent->SetMaterial(0, EffectWithDecalData->DecalMaterial);
	HitDecalComponent->SetRelativeTransform(EffectWithDecalData->DecalTransform);
}
