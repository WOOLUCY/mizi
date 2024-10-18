// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BasicGameMode.h"
#include "Framework/BasicPlayerController.h"
#include "PlayerCharacter/BasicCharacter.h"

ABasicGameMode::ABasicGameMode()
{
	PlayerControllerClass = ABasicPlayerController::StaticClass();
	DefaultPawnClass = ABasicCharacter::StaticClass();
}
