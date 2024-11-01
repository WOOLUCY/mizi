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

	UFUNCTION()
	void OnRandomMapCompleted();

public:
	// 레벨에 스폰된 아이템을 저장하는 배열
	UPROPERTY(BlueprintReadWrite)
	TMap<AItemBase*, bool> SpawnedItems;

	// 레벨 전체에 배치될 아이템 개수의 상한선
	UPROPERTY(BlueprintReadWrite)
	int32 MaxItemAmount = 30;

	UPROPERTY(BlueprintReadWrite)
	bool bDungeonComplete = false;
};
