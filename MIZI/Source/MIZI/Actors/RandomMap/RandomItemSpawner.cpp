// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RandomMap/RandomItemSpawner.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

FSpawnItemTableRow::FSpawnItemTableRow()
{

}

// Sets default values
ARandomItemSpawner::ARandomItemSpawner()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	Box->SetupAttachment(RootComponent);

	Box->SetLineThickness(10.0f);
	Box->SetHiddenInGame(false);

	
	static ConstructorHelpers::FClassFinder<AActor> ActorBlueprint(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/RandomMap/BP_TestItem.BP_TestItem_C'"));
	if (ActorBlueprint.Succeeded())
	{
		ItemClass = ActorBlueprint.Class;
	}
}

void ARandomItemSpawner::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{

}

void ARandomItemSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void ARandomItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARandomItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARandomItemSpawner::GetRandomLocationFromBox()
{
	FVector BoxScale = Box->GetScaledBoxExtent();

	double NewX = UKismetMathLibrary::RandomFloatInRange(-1.0 * BoxScale.X, BoxScale.X);
	double NewY = UKismetMathLibrary::RandomFloatInRange(-1.0 * BoxScale.Y, BoxScale.Y);

	FVector NewVector = UKismetMathLibrary::MakeVector(NewX, NewY, 20.0);
	RandomLocation = Box->GetComponentLocation() + NewVector;
}

void ARandomItemSpawner::SetRandomLocation()
{
	GetRandomLocationFromBox();

	FVector EndLocation = RandomLocation - FVector(0.0, 0.0, 200.0);

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // 자신은 무시

	// 라인 트레이스 수행
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, RandomLocation, EndLocation, ECC_Visibility, CollisionParams);

	// 디버그 라인 그리기
	DrawDebugLine(GetWorld(), RandomLocation, EndLocation, FColor::Green, false, 1.0f, 0, 1.0f);

	if(!bHit)
	{
		SetRandomLocation();
	}
}

void ARandomItemSpawner::SpawnItemRandomly()
{
	if (UWorld* World = GetWorld())
	{
		for (uint32 i = 0; i < ItemCount; ++i)
		{
			SetRandomLocation();

			FVector SpawnLocation = RandomLocation;
			FRotator SpawnRotation = FRotator::ZeroRotator; // 회전 없음

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AActor* SpawnedActor = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);


		}
	}
}

