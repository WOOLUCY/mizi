// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Data/GimmickData.h"
#include "GameFramework/Pawn.h"
#include "Enemy/AI/Interface/EnemyInterface.h"

#include "GimmickBase.generated.h"

UCLASS()
class MIZI_API AGimmickBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGimmickBase();

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

public:
	virtual void OnScanned();

private:
	UFUNCTION()
	virtual void OnScanTimer();

private:
	FText GenerateRandomSerialNumber();

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* Widget;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.GimmickTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FGimmickTableRow* GimmickData;

private:
	FText SerialNumber;
	int32 Damage = 5.f;

};
