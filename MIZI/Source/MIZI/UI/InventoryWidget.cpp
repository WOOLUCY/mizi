// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"

#include "Components/Image.h"
#include "PlayerCharacter/BasicCharacter.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetPointVisibility(0);
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryWidget::ChangeImage(uint32 Index, UImage* ImageSlot)
{
	auto Temp = OwningItems.Find(Index);
	if(!Temp)
	{
		ImageSlot->SetBrushFromTexture(nullptr);
		ImageSlot->SetOpacity(0.0);

		return;
	}

	AItemBase* Item = Cast<AItemBase>(*Temp);
	if(Item)
	{
		//ImageSlot->SetBrushFromTexture(Item->GetItemTableRow()->Icon);
		ImageSlot->SetOpacity(1.0);
	}
	else
	{
		ImageSlot->SetBrushFromTexture(nullptr);
		ImageSlot->SetOpacity(0.0);
	}
}

void UInventoryWidget::SetPointVisibility(uint32 PointIndex)
{
	Point01->SetVisibility(ESlateVisibility::Hidden);
	Point02->SetVisibility(ESlateVisibility::Hidden);
	Point03->SetVisibility(ESlateVisibility::Hidden);
	Point04->SetVisibility(ESlateVisibility::Hidden);

	switch (PointIndex)
	{
	case 0:
		Point01->SetVisibility(ESlateVisibility::Visible);
		break;
	case 1:
		Point02->SetVisibility(ESlateVisibility::Visible);
		break;
	case 2:
		Point03->SetVisibility(ESlateVisibility::Visible);
		break;
	case 3:
		Point04->SetVisibility(ESlateVisibility::Visible);
		break;

	default:
			break;
	}
}

void UInventoryWidget::OnInventoryChanged()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(GetOwningPlayerPawn());
	if(!Character)
	{
		check(Character);
		return;
	}

	OwningItems = Character->GetOwningItems();
	ChangeImage(0, Slot01);
	ChangeImage(1, Slot02);
	ChangeImage(2, Slot03);
	ChangeImage(3, Slot04);
	SetToolTipText(Character->GetCurInventoryIndex());
}

void UInventoryWidget::OnInventoryIndexChanged()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(GetOwningPlayerPawn());
	if (!Character)
	{
		ensure(Character);
		return;
	}
	
	SetPointVisibility(Character->GetCurInventoryIndex());
	SetToolTipText(Character->GetCurInventoryIndex());
}

void UInventoryWidget::SetToolTipText(uint32 Index)
{
	auto Temp = OwningItems.Find(Index);
	if(!Temp)
	{
		ToolTip->SetText(FText::FromString(TEXT("")));
		return;
	}

	ABasicCharacter* Character = Cast<ABasicCharacter>(GetOwningPlayerPawn());
	if (!Character)
	{
		ensure(Character);
		return;
	}

	AItemBase* Item = Cast<AItemBase>(*Temp);
	if (Item->GetItemTableRow()->bUsed)
	{
		ToolTip->SetText(FText::FromString(TEXT("PRESS [LMB] TO USE\nPRESS [G] TO DROP")));
	}
	else
	{
		ToolTip->SetText(FText::FromString(TEXT("PRESS [G] TO DROP")));
	}
	
}
