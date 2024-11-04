// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BasicGameState.h"

ABasicGameState::ABasicGameState()
{
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