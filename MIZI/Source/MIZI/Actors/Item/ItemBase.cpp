// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/ScannedItemWidget.h"

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
	// 행 이름으로 데이터 가져오기
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

	// Owner 정보 세팅
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

	// TODO: 위젯 표시 이름 변경
	UScannedItemWidget* ItemWidget = Cast<UScannedItemWidget>(Widget->GetWidget());
	ItemWidget->ItemName->SetText(FText::FromString(ItemName));

	Price = UKismetMathLibrary::RandomIntegerInRange(ItemTableRow->MinPrice, ItemTableRow->MaxPrice);
	ItemWidget->ItemPrice->SetText(FText::AsNumber(Price));
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::OnScanned()
{
	// TODO: 위젯 가시성 변경, 스캔 이펙트 재생

}

void AItemBase::OnUsed()
{
}