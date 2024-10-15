// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditAnywhere, Category = "RandomMap")
	uint32 MaxRoomAmount = 10.0f;

	UPROPERTY(EditAnywhere, Category = "RandomMap")
	float MaxDungeonTime;

	UPROPERTY(EditAnywhere, Category = "RandomMap")
	TSubclassOf<AMasterRoom> StartRoomClass;

	UPROPERTY(EditAnywhere, Category = "RandomMap")
	TArray<TSubclassOf<AMasterRoom>> RoomList;

	UPROPERTY(EditAnywhere, Category = "RandomMap")
	TSubclassOf<AMasterRoom> WallClass;

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

private:
	uint32 CurRoomAmount;
	bool bRandomMapComplete = false;

	FTimerHandle RandomMapTimerHandle;

	FTimerHandle DelayTimerHandle;
};
