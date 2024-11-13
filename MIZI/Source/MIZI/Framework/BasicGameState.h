// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item/ItemBase.h"
#include "Actors/RandomMap/RandomMapGenerator.h"
#include "GameFramework/GameStateBase.h"
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

public:
	UFUNCTION()
	void OnRandomMapCompleted();

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

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.ItemTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FItemTableRow* ItemData = nullptr;


	// Time System
private:
	float CurrentTime = 0.0f;
	float SecondsInGameDay = 900.0f;
	float ElapsedHours = 0.0f;
};
