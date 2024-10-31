// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/ScannedItemWidget.h"
#include "TimerManager.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	//StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(StaticMeshComponent);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	// TODO: 위젯 로드 하드코딩 부분 변경
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Item/Widget/WBP_ScannedItem.WBP_ScannedItem_C'"));
	if (WidgetClass.Succeeded())
	{
		Widget->SetWidgetClass(WidgetClass.Class);
	}
	//Widget->SetWidgetClass(UScannedItemWidget::StaticClass());

	Widget->SetupAttachment(StaticMeshComponent);
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetPivot(FVector2D(0.3, 0.3));
}

void AItemBase::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	// �� �̸����� ������ ��������
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FItemTableRow* Data = DataTableRowHandle.GetRow<FItemTableRow>(ItemName);
	if (!Data) { ensure(false); return; }

	ItemTableRow = Data;

	StaticMeshComponent->SetStaticMesh(Data->StaticMesh);
	//check(Data->AnimClass);
	//USkeletalMeshComponent* MeshComponent = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	//check(MeshComponent);
	//MeshComponent->SetAnimClass(Data->AnimClass);

	// Owner ���� ����
	{
		//OwningPawn = Cast<APawn>(GetOwner());
		//check(OwningPawn);
		//OwningPawnBodyMesh = OwningPawn->GetComponentByClass<USkeletalMeshComponent>();
		//check(OwningPawnBodyMesh);
		//BasicAnimInstance = Cast<UBaisicCharacterAnimInstance>(MeshComponent->GetAnimInstance());
		//check(BasicAnimInstance);
	}
}


void AItemBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;

	UKismetSystemLibrary::GetComponentBounds(StaticMeshComponent, Origin, BoxExtent, SphereRadius);
	SphereCollision->SetSphereRadius(SphereRadius + 50.0f);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	SetData(DataTableRowHandle);

	UScannedItemWidget* ItemWidget = Cast<UScannedItemWidget>(Widget->GetWidget());
	ItemWidget->ItemName->SetText(ItemTableRow->DisplayName);

	Price = UKismetMathLibrary::RandomIntegerInRange(ItemTableRow->MinPrice, ItemTableRow->MaxPrice);
	ItemWidget->ItemPrice->SetText(FText::AsNumber(Price));

	if (!Widget) { return; }
	Widget->SetVisibility(false);
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::OnScanned()
{
	if (!Widget) { return; }
	Widget->SetVisibility(true);
	UScannedItemWidget* ScannedItemWidget = Cast<UScannedItemWidget>(Widget->GetWidget());
	ScannedItemWidget->PlayAnimation(ScannedItemWidget->ScanEffect);

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnScanTimer"), 3.f, false);
}

void AItemBase::OnUsed()
{
}

void AItemBase::OnScanTimer()
{
	Widget->SetVisibility(false);
	UScannedItemWidget* ScannedItemWidget = Cast<UScannedItemWidget>(Widget->GetWidget());
	//ScannedItemWidget->ScanI
}
