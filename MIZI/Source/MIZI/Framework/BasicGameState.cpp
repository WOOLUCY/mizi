// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BasicGameState.h"

#include "Data/CommandData.h"

ABasicGameState::ABasicGameState()
{
    StoreItemList.Add("Rifle");
    StoreItemList.Add("RifleBullet");
}

void ABasicGameState::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
    SetData(DataTableRowHandle);
}

void ABasicGameState::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
    DataTableRowHandle = InDataTableRowHandle;
    if (DataTableRowHandle.IsNull()) { return; }
    FItemTableRow* Data = DataTableRowHandle.GetRow<FItemTableRow>(TEXT("Rifle"));
    if (!Data) { ensure(false); return; }

    ItemData = Data;
}

void ABasicGameState::BeginPlay()
{
    Super::BeginPlay();

    // TODO: 상품의 가격 랜덤으로 정하기
    for (auto& Iter : StoreItemList)
    {
        Iter.Value = SetItemPrice(DataTableRowHandle, Iter.Key);
    }
}


uint32 ABasicGameState::SetItemPrice(const FDataTableRowHandle& InDataTableRowHandle, FString String)
{
    DataTableRowHandle.RowName = FName(*String);

    if (DataTableRowHandle.DataTable == nullptr || DataTableRowHandle.IsNull())
    {
        ensure(false);
        return -1;
    }

    FItemTableRow* NewData = DataTableRowHandle.GetRow<FItemTableRow>(String);
    if (NewData)
    {
        ItemData = NewData;

        return ItemData->MaxPrice;
    }

    return -1;
}

void ABasicGameState::OnRandomMapCompleted()
{
    UE_LOG(LogTemp, Warning, TEXT("Random Map is generated."));

    UE_LOG(LogTemp, Warning, TEXT("%d Items are spawned."), SpawnedItems.Num());

    uint32 PriceSum = 0;
    for(auto Item : SpawnedItems)
    {
        PriceSum += Item.Key->GetActualPrice();
    }

    UE_LOG(LogTemp, Warning, TEXT("Price: %d"), PriceSum);

    bDungeonComplete = true;
}
