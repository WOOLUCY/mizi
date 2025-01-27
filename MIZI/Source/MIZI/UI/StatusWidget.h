// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Framework/BasicPlayerState.h"
#include "PlayerCharacter/BasicCharacter.h"
#include "StatusWidget.generated.h"

class AItemBase;
/**
 * 
 */
UCLASS()
class MIZI_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void RevealBulletWidget();
	void HideBulletWidget();

public:
	void UpdateWeightText();
	void UpdateMovemnet();

	// Minimap
	void OnMapOn();
	void OnMapOff();

	// Damage
	void OnDamaged();

	// Inventory
public:
	void ChangeSlotText(uint32 Index, UTextBlock* TextSlot);
	void OnInventoryChanged();
	void OnInventoryIndexChanged();
	void SetToolTipText(uint32 Index);

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> SANBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurBulletAmountText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> WeightText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> BatteryText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Minimap;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CautionText;


public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> MapOn;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> MapOff;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FireGun;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Damaged;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Caution;

	// Inventory
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Slot01;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Slot02;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Slot03;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Slot04;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SlotImg01;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SlotImg02;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SlotImg03;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SlotImg04;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ToolTip;

private:
	TObjectPtr<ABasicPlayerState> Status = nullptr;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Slot01Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Slot02Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Slot03Selected;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Slot04Selected;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Slot01Off;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Slot02Off;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Slot03Off;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Slot04Off;

private:
	TMap<uint32, AItemBase*> OwningItems;
};

inline void UStatusWidget::SetToolTipText(uint32 Index)
{
	auto Temp = OwningItems.Find(Index);
	if (!Temp)
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
		ToolTip->SetText(FText::FromString(TEXT("[LMB] TO USE\n[G] TO DROP")));
	}
	else
	{
		ToolTip->SetText(FText::FromString(TEXT("[G] TO DROP")));
	}
}
