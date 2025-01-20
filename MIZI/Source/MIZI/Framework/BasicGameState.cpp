// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BasicGameState.h"

#include "Data/CommandData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/BasicHUD.h"

ABasicGameState::ABasicGameState()
{
    PrimaryActorTick.bCanEverTick = true;

    StoreItemList.Add("Rifle");
    StoreItemList.Add("RifleBullet");
    StoreItemList.Add("Flash");
}

void ABasicGameState::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
    SetData(DataTableRowHandle);
}

void ABasicGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("FactoryLevel"))
    {
        UpdateTime(DeltaTime);
        SpawnEnemys();
    }
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

void ABasicGameState::UpdateTime(float DeltaTime)
{
    CurrentTime += DeltaTime;

    ElapsedHours = (CurrentTime / SecondsInGameDay) * 17.0f;

    int32 Hour = 0.0f;
    int32 Minute = 0.0f;

    Hour = 7 + FMath::FloorToInt(ElapsedHours);
    Minute = FMath::FloorToInt((ElapsedHours - FMath::FloorToInt(ElapsedHours)) * 60);

    FString FormattedTime = FString::Printf(TEXT("%02d:%02d"), Hour, Minute);

    ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
    if (!HUD)
    {
        ensure(false);
        return;
    }

    HUD->GetStatusWidget()->TimeText->SetText(FText::FromString(FormattedTime));

    Battery = 100 - (CurrentTime / SecondsInGameDay * 100);
    FormattedTime = FString::Printf(TEXT("BATT: %02d%%"), Battery);
    HUD->GetStatusWidget()->BatteryText->SetText(FText::FromString(FormattedTime));
}

void ABasicGameState::SpawnEnemys()
{
    // 레벨에 배치된 모든 방을 찾고, 거기서 임의의 방을 선택
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterRoom::StaticClass(), FoundActors);
    if (FoundActors.IsEmpty()) return;

    int32 CurHour = UKismetMathLibrary::FFloor(ElapsedHours);
    if (CurHour == PrevHour)   return;      // 시간 단위로 계산
    if (CurHour % EnemySpawnInterval != 0) return;  // 설정한 인터벌만큼 시간이 지났는지 확인

    auto SelectedRoom = FUtils::GetRandomElementFromArray(FoundActors);

    AMasterRoom* Room = Cast<AMasterRoom>(SelectedRoom);
    Room->SpawnRandomEnemy();

    PrevHour = CurHour;
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
