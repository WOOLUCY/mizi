// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


#include "ScannedItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UScannedItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemPrice;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ScanEffect;
};
