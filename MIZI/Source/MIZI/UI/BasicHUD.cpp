// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BasicHUD.h"
#include "PlayerCharacter/BasicCharacter.h"

ABasicHUD::ABasicHUD()
{

	// TOOD: 위젯 블루프린트를 하드코딩으로 가져오는 부분 수정
	ConstructorHelpers::FClassFinder<UInventoryWidget>  InventoryWidgetObject(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Character/Widget/WBP_Inventory.WBP_Inventory_C'"));
	if (InventoryWidgetObject.Succeeded())
	{
		InventoryWidgetClass = InventoryWidgetObject.Class;
	}

	ConstructorHelpers::FClassFinder<UStatusWidget>  StatusWidgetObject(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Character/Widget/WBP_HUD.WBP_HUD_C'"));
	if (StatusWidgetObject.Succeeded())
	{
		StatusWidgetClass = StatusWidgetObject.Class;
	}

}

void ABasicHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ABasicHUD::BeginPlay()
{
	Super::BeginPlay();
	//ABasicCharacter* Character = Cast<ABasicCharacter>(GetOwningPawn());
	//auto test = Character->GetCharacterData()->InventoryWidgetClass;

	//InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	//if(InventoryWidget)
	//{
	//	InventoryWidget->AddToViewport();
	//}

	StatusWidget = CreateWidget<UStatusWidget>(GetWorld(), StatusWidgetClass);
	if (StatusWidget)
	{
		StatusWidget->AddToViewport();
	}

}

void ABasicHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
