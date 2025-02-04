// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/GunBase.h"

#include "UI/ScannedItemWidget.h"

AGunBase::AGunBase()
{
	ItemName = TEXT("SMG");

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Item/Widget/WBP_ScannedItem.WBP_ScannedItem_C'"));
	if (WidgetClass.Succeeded())
	{
		Widget->SetWidgetClass(WidgetClass.Class);
	}
}

void AGunBase::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	GunDataTableRowHandle = InDataTableRowHandle;
	if (GunDataTableRowHandle.IsNull()) { return; }
	FGunTableRow* Data = GunDataTableRowHandle.GetRow<FGunTableRow>(ItemName);
	if (!Data) { ensure(false); return; }

	GunTableRow = Data;

	StaticMeshComponent->SetStaticMesh(Data->StaticMesh);
}

void AGunBase::OnConstruction(const FTransform& Transform)
{
	//Super::OnConstruction(Transform);
	SetData(GunDataTableRowHandle);
}

void AGunBase::BeginPlay()
{
	SetData(GunDataTableRowHandle);

	UScannedItemWidget* ItemWidget = Cast<UScannedItemWidget>(Widget->GetWidget());

	if (ItemWidget)
	{
		ItemWidget->ItemName->SetText(GunTableRow->DisplayName);
		ItemWidget->ItemPrice->SetText(FText::AsNumber(GunTableRow->Price));
	}

	Price = GunTableRow->Price;
	Weight = GunTableRow->Weight;

	if (Widget)
	{
		Widget->SetVisibility(false); return;
	}
}
