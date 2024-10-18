// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BasicPlayerController.h"

ABasicPlayerController::ABasicPlayerController()
{
}

void ABasicPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ABasicPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABasicPlayerController::OnMove(const FInputActionValue& InputActionValue)
{
}

void ABasicPlayerController::OnLook(const FInputActionValue& InputActionValue)
{
}

void ABasicPlayerController::OnJump(const FInputActionValue& InputActionValue)
{
}
