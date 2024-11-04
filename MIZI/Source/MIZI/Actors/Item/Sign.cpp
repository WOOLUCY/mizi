// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item/Sign.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/BasicCharacter.h"

ASign::ASign()
{
	ItemName = TEXT("Sign");

	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	HitSphere->SetupAttachment(StaticMeshComponent);

	HitSphere->OnComponentBeginOverlap.AddDynamic(this, &ASign::OnOverlapBegin);
}

void ASign::OnUsed()
{
	if (bIsUsed)	return;

	SetActorEnableCollision(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ABasicCharacter* Player = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Player) return;

	// TODO: 사운드 변경
	if(SwingSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SwingSound);
	}

	float Duration = Player->OnSignAttack(OverlappedActors);

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("EmptyOverlappedActors"), Duration, false);
	bIsUsed = false;

	Super::OnUsed();
}

void ASign::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!Cast<ABasicCharacter>(OtherActor))
	{
		OverlappedActors.Add(OtherActor);
	}
}

void ASign::EmptyOverlappedActors()
{
	OverlappedActors.Empty();
}
