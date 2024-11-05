// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatusWidget.h"

#include "Framework/BasicPlayerState.h"

void UStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
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
	{
		//TODO: 남은 총알 업데이트
	}
}
