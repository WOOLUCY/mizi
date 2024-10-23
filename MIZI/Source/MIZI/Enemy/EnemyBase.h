// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Data/EnemyData.h"

#include "EnemyBase.generated.h"

class ABasicAIController;
struct FEnemyTableRow;

UCLASS()
class MIZI_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FEnemyTableRow* GetEnemyData() const { return EnemyData; }

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.EnemyTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FEnemyTableRow* EnemyData;
};
