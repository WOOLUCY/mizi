// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Misc/Utils.h"
#include "Data/CharacterData.h"
#include "Components/SoftWheelSpringArmComponent.h"


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


protected:	// Components
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USoftWheelSpringArmComponent> SpringArm;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.CharacterTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FCharacterTableRow* CharacterData;
};
