// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Actors/RandomMap/RandomMapGenerator.h"
#include "GameFramework/GameStateBase.h"
#include "Enemy/EnemyBase.h"

#include "BasicGameState.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ABasicGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABasicGameState();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	uint32 SetItemPrice(const FDataTableRowHandle& InDataTableRowHandle, FString String);
	void UpdateTime(float DeltaTime);
	void SpawnEnemys();

public:
	UFUNCTION()
	void OnRandomMapCompleted();

	UFUNCTION()
	TMap<UClass*, int32>& GetEnemyList() { return EnemyList; }

public:
	// 레벨에 스폰된 아이템을 저장하는 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<AItemBase*, bool> SpawnedItems;

	// 레벨 전체에 배치될 아이템 개수의 상한선
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxItemAmount = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDungeonComplete = false;

	// Store에서 팔 제품의 목록과 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> StoreItemList;

	// 레벨에 스폰될 적과 그 상한
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UClass*, int32> EnemyList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ElapsedHours = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Battery = 100;

	float CurrentTime = 0.0f;
	float SecondsInGameDay = 60.0f * 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemySpawnInterval = 2;

	int32 PrevHour = 0;
protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.ItemTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FItemTableRow* ItemData = nullptr;
};
