// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatusWidget.h"

#include "Actors/Weapon/Rifle.h"
#include "Framework/BasicPlayerState.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/BasicCharacter.h"

void UStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CurBulletAmountText->SetVisibility(ESlateVisibility::Hidden);

	OnInventoryChanged();
	OnInventoryIndexChanged();
	//OnMapOff();
}

void UStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	ABasicPlayerState* PlayerState = Cast<ABasicPlayerState>(GetOwningPlayerState());

	if(PlayerState)
	{
		Status = PlayerState;
	}
}

void UStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	{
		float Percent = ((float)(Status->GetCurStamina())) / ((float)(Status->GetMaxStamina()));
		StaminaBar->SetPercent(Percent);
	}
	{
		float Percent = ((float)(Status->GetCurHealth())) / ((float)(Status->GetMaxHealth()));
		HPBar->SetPercent(Percent);
	}
	{
		float Percent = ((float)(Status->GetCurSanity())) / ((float)(Status->GetMaxSanity()));
		SANBar->SetPercent(Percent);
	}

	UpdateMovemnet();
}

void UStatusWidget::RevealBulletWidget()
{
	//TODO: 남은 총알 업데이트
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	OwningItems = Character->GetOwningItems();
	//ARifle* Rifle = Cast<ARifle>(OwningItems[Character->GetCurInventoryIndex()]);
	//if (!Rifle)
	//{
	//	CurBulletAmountText->SetVisibility(ESlateVisibility::Hidden);
	//	return;
	//}

	//int32 BulletAmount = Rifle->GetCurBulletAmount();
	//FString String = FString::FromInt(BulletAmount);
	//CurBulletAmountText->SetText(FText::FromString(String));
	//CurBulletAmountText->SetVisibility(ESlateVisibility::Visible);

	AGunBase* SMG = Cast<AGunBase>(OwningItems[Character->GetCurInventoryIndex()]);
	if (!SMG)
	{
		CurBulletAmountText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	int32 BulletAmount = SMG->GetCurBulletAmount();
	FString String = FString::FromInt(BulletAmount);
	CurBulletAmountText->SetText(FText::FromString(String));
	CurBulletAmountText->SetVisibility(ESlateVisibility::Visible);
	
}

void UStatusWidget::HideBulletWidget()
{
	CurBulletAmountText->SetVisibility(ESlateVisibility::Hidden);
}

void UStatusWidget::UpdateWeightText()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	WeightText->SetText(FText::AsNumber(Character->GetWeight()));
}

void UStatusWidget::UpdateMovemnet()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}



	if(Character->GetVelocity().IsNearlyZero())
	{
		MoveText->SetText(FText::FromString(TEXT("IDLE")));
	}
	else if (Character->GetMovementComponent()->IsFalling())
	{
		MoveText->SetText(FText::FromString(TEXT("JUMP")));
	}
	else if (Character->GetIsSprinting())
	{
		MoveText->SetText(FText::FromString(TEXT("DASH")));
	}
	else if(Character->GetMovementComponent()->IsCrouching())
	{
		MoveText->SetText(FText::FromString(TEXT("CRCH")));
	}
	else
	{
		MoveText->SetText(FText::FromString(TEXT("WALK")));
	}
}

void UStatusWidget::UpdateBulletText()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	OwningItems = Character->GetOwningItems();

	AGunBase* SMG = Cast<AGunBase>(OwningItems[Character->GetCurInventoryIndex()]);
	if (!SMG)
	{
		CurBulletAmountText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	int32 BulletAmount = SMG->GetCurBulletAmount();
	FString String = FString::FromInt(BulletAmount);
	CurBulletAmountText->SetText(FText::FromString(String));
}

void UStatusWidget::OnMapOn()
{
	if(MapOn)
	{
		PlayAnimation(MapOn);
	}
}

void UStatusWidget::OnMapOff()
{
	if (MapOff)
	{
		PlayAnimation(MapOff);
	}
}

void UStatusWidget::OnDamaged()
{
	PlayAnimation(Damaged);
	CautionText->SetText(FText::FromString("DAMAGED"));
	PlayAnimation(Caution, 0.0, 5);
}

void UStatusWidget::ChangeSlotText(uint32 Index, UTextBlock* TextSlot)
{
	auto Temp = OwningItems.Find(Index);
	if(!Temp)
	{
		TextSlot->SetText(FText::FromString(""));
		return;
	}

	AItemBase* Item = Cast<AItemBase>(*Temp);
	if (Item)
	{
		TextSlot->SetText(Item->GetItemTableRow()->DisplayName);
	}
	else
	{
		TextSlot->SetText(FText::FromString(""));
	}
}

void UStatusWidget::OnInventoryChanged()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(GetOwningPlayerPawn());
	if (!Character)
	{
		ensure(Character);
		return;
	}

	OwningItems = Character->GetOwningItems();
	ChangeSlotText(0, Slot01);
	ChangeSlotText(1, Slot02);
	ChangeSlotText(2, Slot03);
	ChangeSlotText(3, Slot04);
	SetToolTipText(Character->GetCurInventoryIndex());
}

void UStatusWidget::OnInventoryIndexChanged()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(GetOwningPlayerPawn());
	if (!Character)
	{
		ensure(Character);
		return;
	}

	int CurIdx = Character->GetCurInventoryIndex();
	switch (CurIdx)
	{
	case 0:
		PlayAnimation(Slot01Selected);
		break;
	case 1:
		PlayAnimation(Slot02Selected);
		break;
	case 2:
		PlayAnimation(Slot03Selected);
		break;
	case 3:
		PlayAnimation(Slot04Selected);
		break;
	}

	int PrevIdx = Character->GetPrevInventoryIndex();
	switch (PrevIdx)
	{
	case 0:
		PlayAnimation(Slot01Off);
		break;
	case 1:
		PlayAnimation(Slot02Off);
		break;
	case 2:
		PlayAnimation(Slot03Off);
		break;
	case 3:
		PlayAnimation(Slot04Off);
		break;
	}

	SetToolTipText(Character->GetCurInventoryIndex());


}
