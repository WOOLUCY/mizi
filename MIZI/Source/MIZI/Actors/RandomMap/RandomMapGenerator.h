// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorBase.h"
#include "EndWall.h"
#include "Actors/Item/ItemBase.h"
#include "GameFramework/Actor.h"
#include "Actors/RandomMap/MasterRoom.h"

#include "RandomMapGenerator.generated.h"

// Data Asset For Random Map Generator
// - Start Room Class
// - Wall Class
// - Room List
// - Room Amount
// - MaxDungeonTime

UCLASS()
class MIZI_API URandomMapGeneratorDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "RandomMap|Room")
	int32 Seed = -1;

	UPROPERTY(EditAnywhere, Category = "RandomMap|Room")
	uint32 MaxRoomAmount = 10;

	UPROPERTY(EditAnywhere, Category = "RandomMap|Room")
	float MaxDungeonTime;

	UPROPERTY(EditAnywhere, Category = "RandomMap|Room")
	TSubclassOf<AMasterRoom> StartRoomClass;

	UPROPERTY(EditAnywhere, Category = "RandomMap|Room")
	TArray<TSubclassOf<AMasterRoom>> RoomList;

	UPROPERTY(EditAnywhere, Category = "RandomMap|Room")
	TArray<TSubclassOf<ADoorBase>> DoorList;

	UPROPERTY(EditAnywhere, Category = "RandomMap|Room")
	TSubclassOf<AEndWall> WallClass;

	// Random Item Spawn

	// 스폰될 아이템의 종류
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomMap|Item")
	TArray<TSubclassOf<AItemBase>> ItemList;

	// 방 하나 당 스폰될 아이템 개수의 상한선
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomMap|Item")
	int32 MaxItemAmount = 5;

	// 방 하나 당 스폰될 아이템 개수의 하한선
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RandomMap|Item")
	int32 MinItemAMount = 0;
};

UCLASS()
class MIZI_API ARandomMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void SpawnStartRoom();
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void StartRandomMapTimer();
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void CheckToRandomMapComplete();
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void SpawnNextRoom();
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void CheckForOverlap();
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void AddOverlappingRoomsToList();
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void CloseHoles();
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void SetSeed();

	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void InitializeItemSpawning();
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void SpawnItems();
	UFUNCTION(BlueprintCallable, Category = "RandomMap")
	void GetRandomPointsFromSpawnBox(int32 MaxAttempts = 10);


	uint32 GetRoomAmount() { return CurRoomAmount; };
	void SetRoomAmount(uint32 _InAmount) { CurRoomAmount = _InAmount; }

	bool GetDungeonComplete() { return bRandomMapComplete; }
	void SetDungeonComplete(bool _InBool) { bRandomMapComplete = _InBool; }

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(VisibleAnywhere)
	UClass* StartRoomClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USceneComponent*> ExitsList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SelectedExitPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AMasterRoom> LatestRoom;

	UPROPERTY(EditAnywhere)
	class URandomMapGeneratorDataAsset* RandomMapGeneratorDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPrimitiveComponent*> OverlappedList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRandomStream Stream;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* SpawnBox;



private:
	uint32 CurRoomAmount;
	bool bRandomMapComplete = false;

	FTimerHandle RandomMapTimerHandle;

	FVector RandomBoxLocation;
};