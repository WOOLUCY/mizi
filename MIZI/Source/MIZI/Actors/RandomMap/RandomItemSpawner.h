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

	// TODO: ���ڿ��� �ƴ� ����Ŭ������ Ű������ �ް� ������ ���̺� ����
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


	// TODO: ������ ���̺��� ����ϴ� �ڵ�� ���� ��, �ش� ��� �������� ���� ��
	UPROPERTY(EditAnywhere)
	UClass* ItemClass;

	UPROPERTY(EditAnywhere)
	uint32 ItemCount;

protected:
	FSpawnItemTableRow* SpawnItemTableRow = nullptr;

private:
	FVector RandomLocation;

};
