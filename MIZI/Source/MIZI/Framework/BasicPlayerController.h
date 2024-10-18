// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Misc/Utils.h"

#include "BasicPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ABasicPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABasicPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn);

protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OnLook(const FInputActionValue& InputActionValue);
	void OnJump(const FInputActionValue& InputActionValue);

protected:
	UInputMappingContext* IMC_Default = nullptr;
};
