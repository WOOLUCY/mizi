// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Actors/Gimmick/GimmickBase.h"
#include "Actors/Item/ItemBase.h"
#include "Actors/RandomMap/LockedDoor.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Misc/Utils.h"
#include "Data/CharacterData.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Framework/BasicPlayerState.h"
#include "GameFramework/SpringArmComponent.h"


#include "BasicCharacter.generated.h"

UCLASS()
class MIZI_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void ChangeControllerRotationYaw();

protected:
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust);
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust);

public:
	void OnScan();
	void OnChangePerspective();
	void OnPickUpItem();
	void OnDropItem();
	void OnInventoryChanged();
	void OnInventoryIndexChanged(float Value);
	void OnEquipChanged();
	void StartSprinting();
	void StopSprinting();
	void OnUseItem();

	void OnTerminalPressed();
	void OnTerminalReleased();

	float OnSignAttack(TSet<AActor*> DamagedActors);

public:
	FCharacterTableRow* GetCharacterData() const { return CharacterData; }
	TMap<uint32, AItemBase*>& GetOwningItems() { return OwningItems; }
	uint32 GetCurInventoryIndex() const { return CurInventoryIndex; }
	uint32 GetPrevInventoryIndex() const { return PrevInventoryIndex; }
	bool GetIsSprinting() const { return bIsSprinting; }
	void SetIsSprinting(bool InBool) { bIsSprinting = InBool; }

	FVector GetCameraWorldLocation() { return FirstPersonCamera->K2_GetComponentLocation(); }
	FVector GetCameraForwardVector() {return FirstPersonCamera->GetForwardVector();}

	bool GetCanUseConsole() const { return bCanUseConsole; }
	void SetCanUseConsole(const bool InBool) { bCanUseConsole = InBool; }

	float GetWeight() const { return Weight; }
	void SetWeight(float In) { Weight = In; }

protected:	// Components
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	//TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> ThirdPersonCamera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTimelineComponent> ScanTimelineComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ABasicPlayerState> Status;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetInteractionComponent> WidgetInteraction;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.CharacterTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FCharacterTableRow* CharacterData;

private:
	bool bUseFirstPersonCamera = true;
	FTimeline ScanTimeLine;

	TArray<AItemBase*> ScannedItems;
	TArray<AGimmickBase*> ScannedGimmicks;


protected:	// Inventory
	uint32 CurInventoryIndex = 0;
	uint32 MaxInventoryIndex = 4;
	uint32 PrevInventoryIndex = 0;
	TMap<uint32, AItemBase*> OwningItems;

	AItemBase* OverlappedItem = nullptr;

private:	// Sprint
	bool bIsSprinting = false;

	UFUNCTION()
	void DrainStamina();
	UFUNCTION()
	void RegenStamina();
protected:
	UFUNCTION()
	void ScanRadiusUpdate(float Radius);

	UFUNCTION()
	void OnScanFinished();

public:
	bool GetCanOpenDoor() const { return bCanOpenDoor; }
	void SetCanOpenDoor(const bool InBool) {bCanOpenDoor = InBool;}

	void SetOverlappedDoor(ALockedDoor* LockedDoor) { OverlappedDoor = LockedDoor; }
	ALockedDoor* GetOverlappedDoor()const { return OverlappedDoor; }

private:
	bool bCanOpenDoor = false;
	ALockedDoor* OverlappedDoor;


	// Parallax HUD
private:
	//UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* ParallaxMPC;

	void SetParallaxHUDOffset();
	float YawRate;
	float PitchRate;
	float MinMaxYawRate = 10.f;
	float MinMaxPitchRate = 10.0f;
	FRotator CameraRotator;
	float PitchParallaxOffset;
	float YawParallaxOffset;
	float InterpSpeed = 10.0f;

protected:
	bool bCanUseConsole = false;

	UPROPERTY(BlueprintReadWrite)
	float Weight = 0.f;

private:
	float CalculatePlayerSpeed();
};
