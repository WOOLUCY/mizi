// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/InventoryWidget.h"
#include "BasicHUD.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ABasicHUD : public AHUD
{
	GENERATED_BODY()
public:
	ABasicHUD();

protected:
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	
public:
	UInventoryWidget* GetInventoryWidget() {return InventoryWidget;}


private:
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
};
