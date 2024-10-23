// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Misc/Utils.h"
#include "Data/CharacterData.h"
#include "Components/SoftWheelSpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "Framework/BasicPlayerState.h"


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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void ChangeControllerRotationYaw();


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

	FCharacterTableRow* GetCharacterData() const { return CharacterData; }
	TMap<uint32, AItemBase*> GetOwningItems() { return OwningItems; }
	uint32 GetCurInventoryIndex() const { return CurInventoryIndex; }
	bool GetIsSprinting() const { return bIsSprinting; }
	void SetIsSprinting(bool InBool) { bIsSprinting = InBool; }


protected:	// Components
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	//TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<UCameraComponent> ThirdPersonCamera;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<USoftWheelSpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<UTimelineComponent> ScanTimelineComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<ABasicPlayerState> Status;


protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.CharacterTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FCharacterTableRow* CharacterData;

private:
	bool bUseFirstPersonCamera = true;
	FTimeline ScanTimeLine;

	TArray<AItemBase*> ScannedItems;

private:	// Inventory
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
};
