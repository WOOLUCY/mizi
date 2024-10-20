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


protected:
	void OnScanned();
	virtual void OnUsed();

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* Widget;

protected: // Owner
	//// 이 아이템의 소유권자 Pawn
	//APawn* OwningPawn = nullptr;
	//// 이 아이템의 소유권자 Pawn의 SkeletalMeshComponent로 캐릭터의 MeshComponent 같은 것을 의미
	//USkeletalMeshComponent* OwningPawnBodyMesh = nullptr;
	////UBaisicCharacterAnimInstance* BasicAnimInstance = nullptr;

protected:
	UPROPERTY(EditAnywhere)
	FString ItemName = TEXT("Flash");

	UPROPERTY(VisibleAnywhere)
	uint32 Price = 5;

	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.ItemTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FItemTableRow* ItemTableRow = nullptr;
};
