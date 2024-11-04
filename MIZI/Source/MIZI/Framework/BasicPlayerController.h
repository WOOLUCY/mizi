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
	void OnLookUp(const FInputActionValue& InputActionValue);
	void OnTurn(const FInputActionValue& InputActionValue);
	void OnJump(const FInputActionValue& InputActionValue);
	void OnStartSprinting(const FInputActionValue& InputActionValue);
	void OnStopSprinting(const FInputActionValue& InputActionValue);

	void OnZoomWheel(const FInputActionValue& InputActionValue);
	void OnZoomIn(const FInputActionValue& InputActionValue);
	void OnZoomOut(const FInputActionValue& InputActionValue);

	void OnScan(const FInputActionValue& InputActionValue);
	void OnChangePerspective(const FInputActionValue& InputActionValue);

	void OnPickItem(const FInputActionValue& InputActionValue);
	void OnDropItem(const FInputActionValue& InputActionValue);

	void OnInventoryWheel(const FInputActionValue& InputActionValue);

	void OnUseItem(const FInputActionValue& InputActionValue);

protected:
	UInputMappingContext* IMC_Default = nullptr;

	class USoftWheelSpringArmComponent* SpringArm = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool bZoomWheel = true;
};
