// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RandomMap/MasterRoom.h"

// Sets default values
AMasterRoom::AMasterRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// DefaultSceneRoot
	//	Arrow
	//	GeometryFolder
	//	OverlapFolder
	//		OverlapBox
	//	ExitsFolder

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->ArrowColor = FColor::Red;
	Arrow->ArrowSize = 8.f;
	Arrow->SetRelativeLocation(FVector(760.0, 0.0, 290.0));
	Arrow->SetHiddenInGame(true);

	GeometryFolder = CreateDefaultSubobject<USceneComponent>(TEXT("GeometryFolder"));
	GeometryFolder->SetupAttachment(RootComponent);

	OverlapFolder = CreateDefaultSubobject<USceneComponent>(TEXT("OverlapFolder"));
	OverlapFolder->SetupAttachment(RootComponent);

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(OverlapFolder);
	OverlapBox->ShapeColor = FColor::Magenta;
	OverlapBox->SetLineThickness(2.f);

	//// Box 컴포넌트의 크기 설정
	//BoxComponent->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f)); // X, Y, Z 크기 설정

	//// 콜리전 설정
	//BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	if(OverlapBox)
	{
		// 프로파일 Util 헤더로 빼기
		OverlapBox->SetCollisionProfileName(TEXT("Room"));
	}

	ExitsFolder = CreateDefaultSubobject<USceneComponent>(TEXT("ExitsFolder"));
	ExitsFolder->SetupAttachment(RootComponent);

	SpawnFolder = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnFolder"));
	SpawnFolder->SetupAttachment(RootComponent);

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetupAttachment(SpawnFolder);
	SpawnBox->ShapeColor = FColor::Cyan;
	SpawnBox->SetLineThickness(2.f);
}

// Called when the game starts or when spawned
void AMasterRoom::BeginPlay()
{
	Super::BeginPlay();

	Arrow->SetHiddenInGame(true);

	
}

// Called every frame
void AMasterRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

