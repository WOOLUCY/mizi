// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RandomMap/LockedDoor.h"

#include "PlayerCharacter/BasicCharacter.h"

ALockedDoor::ALockedDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(RootComponent);

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALockedDoor::OnBoxBeginOverlapped);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALockedDoor::OnBoxEndOverlapped);

	OpenTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}

void ALockedDoor::OnOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("The Door is Opening"));
	if (!bIsLocked) return;
	OpenTimeline->PlayFromStart();
	bIsLocked = false;
}

void ALockedDoor::OnOpenProgress(float Value)
{
	LeftDoor->AddLocalOffset(FVector(Value * -1.f, 0, 0));
	RightDoor->AddLocalOffset(FVector(Value, 0, 0));
}

void ALockedDoor::OnOpenFinished()
{
}

void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("OnOpenProgress"));
	OpenTimeline->AddInterpFloat(OpenTimeCurveFloat, ProgressFunction);

	// 타임라인이 끝났을 때 호출될 함수 바인딩
	FOnTimelineEvent TimelineFinished;
	TimelineFinished.BindUFunction(this, FName("OnOpenFinished"));
	OpenTimeline->SetTimelineFinishedFunc(TimelineFinished);
}

void ALockedDoor::OnBoxBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(OtherActor);
	if (!Character)	return;
	Character->SetCanOpenDoor(true);
	Character->SetOverlappedDoor(this);
}

void ALockedDoor::OnBoxEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(OtherActor);
	if (!Character)	return;
	Character->SetCanOpenDoor(false);
}
