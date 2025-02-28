// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BasicPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter/BasicPlayerCameraManager.h"
#include "GameFramework/Character.h"
#include "Components/SoftWheelSpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/BasicCharacter.h"
#include "UI/BasicHUD.h"


ABasicPlayerController::ABasicPlayerController()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Basic.IMC_Basic'") };
		check(Asset.Object);

		IMC_Default = Asset.Object;
	}

	PlayerCameraManagerClass = ABasicPlayerCameraManager::StaticClass();
}

void ABasicPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//SpringArm = GetPawn()->GetComponentByClass<USoftWheelSpringArmComponent>();
	//check(SpringArm);
	//SpringArm->SetMinMaxTargetArmLength(160.f, 260.f);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(IMC_Default, 0);
}

void ABasicPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);

	// Move
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Move")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMove); 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Move is disabled"));
	}

	// LookUp
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_LookUp")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnLookUp);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_LookMouse is disabled"));
	}

	// Turn
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Turn")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnTurn);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_LookMouse is disabled"));
	}

	// Jump
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Jump")))
	{
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Started, this, &ThisClass::OnJump);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Jump is disabled"));
	}

	// Zoom
	//if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Zoom")))
	//{
	//	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnZoomIn);
	//	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnZoomOut);
	//}
	//else
	//{
	//	ensureMsgf(false, TEXT("IA_Zoom is disabled"));
	//}


	// ZoomWheel
	//if (bZoomWheel)
	//{
	//	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_ZoomWheel")))
	//	{
	//		EnhancedInputComponent->BindAction(InputAction,
	//			ETriggerEvent::Triggered, this, &ThisClass::OnZoomWheel);
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("IA_ZoomWheel is disabled"));
	//	}
	//}

	// Scan
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Scan")))
	{
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Started, this, &ThisClass::OnScan);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Scan is disabled"));
	}

	// Change Perspective
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Perspective")))
	{
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Started, this, &ThisClass::OnChangePerspective);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Perspective is disabled"));
	}

	// Pick
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Pick")))
	{
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Triggered, this, &ThisClass::OnPickItem);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Pick is disabled"));
	}

	// Drop
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Drop")))
	{
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Triggered, this, &ThisClass::OnDropItem);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Drop is disabled"));
	}

	// Inventory Wheel
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_InventoryWheel")))
	{
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Triggered, this, &ThisClass::OnInventoryWheel);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_InventoryWheel is disabled"));
	}

	// Sprinting
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Dash")))
	{
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Triggered, this, &ThisClass::OnStartSprinting);
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Completed, this, &ThisClass::OnStopSprinting);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Dash is disabled"));
	}

	// Item Use
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_ItemUse")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnUseItemTriggered);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnUseItem);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_ItemUse is disabled"));
	}

	// Crouch
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Crouch")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnCrouch);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnUnCrouch);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Crouch is disabled"));
	}

	// Terminal Click
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_TerminalClick")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnTerminalClick);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnTerminalRelease);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_TerminalClick is disabled"));
	}

	// MiniMap
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Default, TEXT("IA_Map")))
	{
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Started, this, &ThisClass::OnMapOn);
		EnhancedInputComponent->BindAction(InputAction,
			ETriggerEvent::Completed, this, &ThisClass::OnMapOff);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Dash is disabled"));
	}
}

void ABasicPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABasicPlayerController::OnMove(const FInputActionValue& InputActionValue)
{
	//if (StatusComponent && !StatusComponent->CanMove()) { return; }

	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = K2_GetActorRotation();
	const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);

	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
	ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);
}

void ABasicPlayerController::OnLookUp(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	float LookUpValue = InputActionValue.Get<float>();
	AddPitchInput(LookUpValue);
}

void ABasicPlayerController::OnTurn(const FInputActionValue& InputActionValue)
{
	// Yaw 축을 기반으로 회전합니다.
	float TurnValue = InputActionValue.Get<float>();
	AddYawInput(TurnValue);
}

void ABasicPlayerController::OnJump(const FInputActionValue& InputActionValue)
{
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	ControlledCharacter->Jump();
}

void ABasicPlayerController::OnStartSprinting(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* ControlledCharacter = Cast<ABasicCharacter>(GetPawn());
	ControlledCharacter->StartSprinting();

}

void ABasicPlayerController::OnStopSprinting(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* ControlledCharacter = Cast<ABasicCharacter>(GetPawn());
	ControlledCharacter->StopSprinting();
}

void ABasicPlayerController::OnZoomWheel(const FInputActionValue& InputActionValue)
{
	//if (!SpringArm) { ensure(false); return; }

	//const float ActionValue = InputActionValue.Get<float>();
	////UE_LOG(LogTemp, Warning, TEXT("Wheel: %f"), ActionValue);
	//if (FMath::IsNearlyZero(ActionValue)) { return; }
	//SpringArm->OnZoomWheel(ActionValue * 20.f);
}

void ABasicPlayerController::OnZoomIn(const FInputActionValue& InputActionValue)
{
	SpringArm->SetDesiredZoom(160.f);
}

void ABasicPlayerController::OnZoomOut(const FInputActionValue& InputActionValue)
{
	SpringArm->SetDesiredZoom(260.f);
}

void ABasicPlayerController::OnScan(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	BasicCharacter->OnScan();
}

void ABasicPlayerController::OnChangePerspective(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	BasicCharacter->OnChangePerspective();
}

void ABasicPlayerController::OnPickItem(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	BasicCharacter->OnPickUpItem();
}

void ABasicPlayerController::OnDropItem(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	BasicCharacter->OnDropItem();
}

void ABasicPlayerController::OnInventoryWheel(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	BasicCharacter->OnInventoryIndexChanged(InputActionValue.Get<float>());
}

void ABasicPlayerController::OnUseItem(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	BasicCharacter->OnUseItem();
}

void ABasicPlayerController::OnUseItemTriggered(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	BasicCharacter->OnUseItemTriggered();
}

void ABasicPlayerController::OnCrouch(const FInputActionValue& InputActionValue)
{
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	if (ControlledCharacter->GetMovementComponent()->IsFalling()) { return; }
	ControlledCharacter->Crouch();
}

void ABasicPlayerController::OnUnCrouch(const FInputActionValue& InputActionValue)
{
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	ControlledCharacter->UnCrouch();
}

void ABasicPlayerController::OnTerminalClick(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	if(BasicCharacter)
	{
		BasicCharacter->OnTerminalPressed();
	}
}

void ABasicPlayerController::OnTerminalRelease(const FInputActionValue& InputActionValue)
{
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(GetPawn());
	if (BasicCharacter)
	{
		BasicCharacter->OnTerminalReleased();
	}
}

void ABasicPlayerController::OnMapOn(const FInputActionValue& InputActionValue)
{
	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	HUD->GetStatusWidget()->OnMapOn();
}

void ABasicPlayerController::OnMapOff(const FInputActionValue& InputActionValue)
{
	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	HUD->GetStatusWidget()->OnMapOff();
}
