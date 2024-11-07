//Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"


#include "TerminalScreenWidget.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETerminalCommand : uint8
{
	HELP = 0	UMETA(DisplayName = "Help"),
	MOONS		UMETA(DisplayName = "Moons"),
	STORE		UMETA(DisplayName = "Store"),
	BESTIARY	UMETA(DisplayName = "Bestiary"),


	UNKNOWN		UMETA(DisplayName = "Unknown"),
};


UCLASS()
class MIZI_API UTerminalScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

protected:
	UFUNCTION()
	void OnInputButtonClicked();

	UFUNCTION()
	void OnInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	void SplitCommand(FText WholeCommand);
	void ExecuteCommand();
	ETerminalCommand GetCommandEnum(const FString& Command);

private:
	void OnHelp();
	void OnMoons();
	void OnStore();
	void OnBestiary();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* OutputText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableText* InputText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* InputButton;

private:
	FString Command;
	FString Argument;

};
