// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


public:
	void ChangeImage(uint32 Index, UImage* ImageSlot);
	void SetPointVisibility(uint32 PointIndex);
	void OnInventoryChanged();
	void OnInventoryIndexChanged();


public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Point01;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Point02;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Point03;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Point04;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Slot01;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Slot02;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Slot03;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Slot04;

private:
	TMap<uint32, AItemBase*> OwningItems;
};
