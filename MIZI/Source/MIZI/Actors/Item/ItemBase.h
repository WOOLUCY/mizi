// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Data/ItemData.h"

#include "ItemBase.generated.h"

UCLASS()
class MIZI_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	virtual void OnScanned();
	virtual void OnUsed();
	virtual void OnPicked();
	virtual void OnDropped();
	virtual void OnUsedTriggered();

	virtual void OnEquiped();
	virtual void OnUnEquiped();

	FItemTableRow* GetItemTableRow() { return ItemTableRow; }

	uint32 GetActualPrice() const { return Price; }
	void SetActualPrice(uint32 In) { Price = In; }

	float GetWeight() const { return Weight; }
	void SetWeight(float In) { Weight = In; }

private:
	UFUNCTION()
	virtual void OnScanTimer();

public:
	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* Widget;

protected: // Owner
	//// �� �������� �������� Pawn
	//APawn* OwningPawn = nullptr;
	//// �� �������� �������� Pawn�� SkeletalMeshComponent�� ĳ������ MeshComponent ���� ���� �ǹ�
	//USkeletalMeshComponent* OwningPawnBodyMesh = nullptr;
	////UBaisicCharacterAnimInstance* BasicAnimInstance = nullptr;

protected:
	UPROPERTY(EditAnywhere)
	FString ItemName = TEXT("Flash");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (RowType = "/Script/MIZI.ItemTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FItemTableRow* ItemTableRow = nullptr;
};
