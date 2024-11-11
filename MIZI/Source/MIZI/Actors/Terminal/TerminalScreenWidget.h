//Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Data/CommandData.h"
#include "Misc/Utils.h"


#include "TerminalScreenWidget.generated.h"

/**
 * 
 */

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
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);


protected:
	UFUNCTION()
	void OnInputButtonClicked();

	UFUNCTION()
	void OnInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	void SplitCommand(FText WholeCommand);
	void ExecuteCommand();
	ETerminalCommand GetCommandEnum(const FString& Command);
	FText ChangeOutputText(const FDataTableRowHandle& InDataTableRowHandle, FString String);

private:
	void OnHelp();
	void OnMoons();
	void OnStore();
	void OnBestiary();
	void OnPurchaseItem();
	void UpdateMoneyText();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* OutputText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableText* InputText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* InputButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MoneyText;

private:
	FString Command;
	FString Argument;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.CommandTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FCommandTableRow* CommandData;

};
