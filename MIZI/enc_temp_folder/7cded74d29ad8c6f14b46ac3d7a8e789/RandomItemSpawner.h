// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

#include "RandomItemSpawner.generated.h"

USTRUCT()
struct MIZI_API FSpawnItemTableRow : public FTableRowBase
{
	GENERATED_BODY()
	FSpawnItemTableRow();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TMap<FString, int32> ItemQuantities;

	// TODO: 문자열이 아닌 서브클래스를 키값으로 받게 데이터 테이블 변경
	//UPROPERTY(EditAnywhere, Category = "SpawnableItem")
	//TMap<TSubclassOf<>
};


UCLASS()
class MIZI_API ARandomItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomItemSpawner();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void GetRandomLocationFromBox();
	void SetRandomLocation();
	void SpawnItemRandomly();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
	UClass* ItemClass;

	UPROPERTY(EditAnywhere)
	uint32 ItemCount;

protected:
	FSpawnItemTableRow* SpawnItemTableRow = nullptr;

private:
	// TODO: 데이터 테이블을 사용하는 코드로 변경 후, 해당 멤버 변수들을 지울 것
	FVector RandomLocation;

};
