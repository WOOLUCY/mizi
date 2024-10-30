// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gimmick/GimmickBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "UI/ScannedGimmickWidget.h"

// Sets default values
AGimmickBase::AGimmickBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCanEverAffectNavigation(false);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(StaticMeshComponent);
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetPivot(FVector2D(0.3, 0.3));
}

// Called when the game starts or when spawned
void AGimmickBase::BeginPlay()
{
	Super::BeginPlay();
	SetData(DataTableRowHandle);

	// 기믹에 랜덤한 일련번호를 부여
	SerialNumber = GenerateRandomSerialNumber();

	UScannedGimmickWidget* ScanWidget = Cast<UScannedGimmickWidget>(Widget->GetWidget());
	if(ScanWidget)
	{
		ScanWidget->DisplayName->SetText(GimmickData->DisplayName);

		ScanWidget->SerialNumber->SetText(SerialNumber);
	}

	if (Widget)
	{
		Widget->SetVisibility(false);
	}
}

void AGimmickBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
}

void AGimmickBase::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FGimmickTableRow* Data = DataTableRowHandle.GetRow<FGimmickTableRow>(TEXT("LandMine"));
	if (!Data) { ensure(false); return; }

	GimmickData = Data;
	if(GimmickData->StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(GimmickData->StaticMesh);
	}

	if(GimmickData->Damage)
	{
		Damage = GimmickData->Damage;
	}

	if(GimmickData->EnemyWidgetClass)
	{
		Widget->SetWidgetClass(GimmickData->EnemyWidgetClass);
	}
}

// Called every frame
void AGimmickBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGimmickBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGimmickBase::OnScanned()
{
	if (!Widget) { return; }
	Widget->SetVisibility(true);
	UScannedGimmickWidget* ScanWidget = Cast<UScannedGimmickWidget>(Widget->GetWidget());
	ScanWidget->PlayAnimation(ScanWidget->ScanEffect);

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnScanTimer"), 3.f, false);
}

void AGimmickBase::OnScanTimer()
{
	Widget->SetVisibility(false);
}

FText AGimmickBase::GenerateRandomSerialNumber()
{
	char RandomChar = 'A' + FMath::RandRange(0, 25);
	int32 RandomNum = FMath::RandRange(0, 99);
	FString RandomSerialNumber = FString::Printf(TEXT("%c%02d"), RandomChar, RandomNum);
	return FText::FromString(RandomSerialNumber);
}

