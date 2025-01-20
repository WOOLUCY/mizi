// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Terminal/Terminal.h"

#include "Framework/BasicGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "PlayerCharacter/BasicCharacter.h"

// Sets default values
ATerminal::ATerminal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBoxComponent"));
	OverlapBox->SetupAttachment(StaticMeshComponent);
	OverlapBox->ShapeColor = FColor::Green;
	OverlapBox->SetLineThickness(2.f);

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBoxComponent"));
	SpawnBox->SetupAttachment(StaticMeshComponent);
	SpawnBox->ShapeColor = FColor::Blue;
	SpawnBox->SetLineThickness(2.f);

	// Screen Widget
	ScreenWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Terminal/WBP_Console.WBP_Console_C'"));
	if (WidgetClass.Succeeded())
	{
		ScreenWidget->SetWidgetClass(WidgetClass.Class);
	}

	ScreenWidget->SetupAttachment(StaticMeshComponent);
	ScreenWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}

// Called when the game starts or when spawned
void ATerminal::BeginPlay()
{
	Super::BeginPlay();

	// Overlap Box Delegate Binding
	if(OverlapBox)
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ATerminal::OnOverlapBoxBeginOverlap);
		OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ATerminal::OnOverlapBoxEndOverlap);
	}

}

// Called every frame
void ATerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATerminal::SpawnOrderdItem(FString Item, FString Amount)
{
	auto Class = ItemLists.Find(Item);

	if (!Class)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnFound Item Class"));
		return;
	}

	int RepeatNum = UKismetStringLibrary::Conv_StringToInt(Amount);

	if (RepeatNum == 0)
	{
		++RepeatNum;
	}


	// 드론을 랜덤한 위치에 스폰
	FVector BoxCenter = SpawnBox->GetComponentLocation();
	FVector BoxScale = SpawnBox->GetScaledBoxExtent();

	float NewX = UKismetMathLibrary::RandomFloatInRange(-BoxScale.X, BoxScale.X);
	float NewY = UKismetMathLibrary::RandomFloatInRange(-BoxScale.Y, BoxScale.Y);
	float NewZ = UKismetMathLibrary::RandomFloatInRange(-BoxScale.Z, BoxScale.Z);
	FVector NewVector = FVector(NewX, NewY, NewZ);
	FVector RandomBoxLocation = BoxCenter + NewVector;
	FTransform SpawnTransform(SpawnBox->GetComponentRotation(), RandomBoxLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(DroneClass, SpawnTransform, SpawnParams);
	if(!SpawnedActor)
	{
		ensure(false);
		return;
	}

	ADrone* Drone = Cast<ADrone>(SpawnedActor);
	if(!Drone)
	{
		ensure(false);
		return;
	}

	Drone->OrderList.Add(*Class, RepeatNum);

	// 지정된 아이템을 수량만큼 스폰
	//for (int i = 1; i <= RepeatNum; ++i)
	//{
	//	FTransform SpawnTransform = SpawnBox->GetComponentToWorld();
	//	FActorSpawnParameters SpawnParams;
	//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(*Class, SpawnTransform, SpawnParams);
	//}
}

void ATerminal::OnOverlapBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(OtherActor);
	if(!Character)
	{
		return;
	}

	Character->SetCanUseConsole(true);
}

void ATerminal::OnOverlapBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	Character->SetCanUseConsole(false);
}


