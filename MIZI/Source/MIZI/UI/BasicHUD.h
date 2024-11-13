// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusWidget.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> StatusWidgetClass;

public:
	UFUNCTION(BlueprintCallable)
	UInventoryWidget* GetInventoryWidget() const {return InventoryWidget;}
	UFUNCTION(BlueprintCallable)
	UStatusWidget* GetStatusWidget() const {return StatusWidget;}

private:
	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY()
	UStatusWidget* StatusWidget;

};
