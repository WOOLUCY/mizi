// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatusWidget.h"

#include "Actors/Item/Rifle.h"
#include "Framework/BasicPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/BasicCharacter.h"

void UStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CurBulletAmountText->SetVisibility(ESlateVisibility::Hidden);
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

	auto OwningItems = Character->GetOwningItems();
	ARifle* Rifle = Cast<ARifle>(OwningItems[Character->GetCurInventoryIndex()]);
	if (!Rifle)
	{
		CurBulletAmountText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	int32 BulletAmount = Rifle->GetCurBulletAmount();
	FString String = FString::FromInt(BulletAmount);
	CurBulletAmountText->SetText(FText::FromString(String));
	CurBulletAmountText->SetVisibility(ESlateVisibility::Visible);
	
}

void UStatusWidget::HideBulletWidget()
{
	CurBulletAmountText->SetVisibility(ESlateVisibility::Hidden);
}
