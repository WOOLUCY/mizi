// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RandomMap/RandomMapGenerator.h"
#include "Engine/World.h"
#include "Actors/RandomMap/MasterRoom.h"
//#include "DataWrappers/ChaosVDQueryDataWrappers.h"
#include "RandomItemSpawner.h"
#include "Framework/BasicGameState.h"
#include "Misc/Utils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARandomMapGenerator::ARandomMapGenerator()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    static ConstructorHelpers::FClassFinder<AActor> ActorBlueprint(TEXT("/Script/Engine.Blueprint'/Game/RandomMap/StartRoom.StartRoom_C'"));
    if (ActorBlueprint.Succeeded())
    {
        StartRoomClass = ActorBlueprint.Class;
    }


    static ConstructorHelpers::FObjectFinder<URandomMapGeneratorDataAsset> DataFinder(TEXT("/Script/MIZI.RandomMapGeneratorDataAsset'/Game/RandomMap/DA_RandomMapGenerator.DA_RandomMapGenerator'"));
    if (DataFinder.Succeeded())
    {
        RandomMapGeneratorDataAsset = DataFinder.Object;
    }
    
    Stream.Initialize(RandomMapGeneratorDataAsset->Seed);
}

// Called when the game starts or when spawned
void ARandomMapGenerator::BeginPlay()
{
    Super::BeginPlay();

    SetRoomAmount(RandomMapGeneratorDataAsset->MaxRoomAmount);
    //SetRoomAmount(10);

    SetSeed();
    SpawnStartRoom();
    StartRandomMapTimer();
    SpawnNextRoom();

    int32 InitialSeed;
    UKismetMathLibrary::BreakRandomStream(Stream, InitialSeed);
    UE_LOG(LogTemp, Warning, TEXT("Seed: %d"), InitialSeed);
}

// Called every frame
void ARandomMapGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ARandomMapGenerator::SpawnStartRoom()
{
    if (UWorld* World = GetWorld())
    {
        FTransform RootTransform = DefaultSceneRoot->K2_GetComponentToWorld();
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* SpawnedActor = World->SpawnActor<AActor>(RandomMapGeneratorDataAsset->StartRoomClass, RootTransform, SpawnParams);

        AMasterRoom* SpawnedRoom = Cast<AMasterRoom>(SpawnedActor);
        TArray<USceneComponent*> OutChildren;
        SpawnedRoom->ExitsFolder->GetChildrenComponents(false, OutChildren);

        ExitsList.Append(OutChildren);
    }
}

void ARandomMapGenerator::SpawnNextRoom()
{
    if (UWorld* World = GetWorld())
    {
        //SelectedExitPoint = FUtils::GetRandomElementFromArray(ExitsList);
        SelectedExitPoint = FUtils::RandomArrayItemFromStream(ExitsList, Stream);

        FTransform SelectedTransform = SelectedExitPoint->K2_GetComponentToWorld();
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        //TSubclassOf<AMasterRoom> RoomClass = FUtils::GetRandomElementFromArray(RandomMapGeneratorDataAsset->RoomList);
        TSubclassOf<AMasterRoom> RoomClass = FUtils::RandomArrayItemFromStream(RandomMapGeneratorDataAsset->RoomList, Stream);

        AActor* SpawnedActor = World->SpawnActor<AActor>(RoomClass, SelectedTransform, SpawnParams);
        LatestRoom = Cast<AMasterRoom>(SpawnedActor);

        UKismetSystemLibrary::K2_SetTimer(this, TEXT("CheckForOverlap"), 0.02f, false);
    }
}


void ARandomMapGenerator::StartRandomMapTimer()
{
    GetWorld()->GetTimerManager().SetTimer(
        RandomMapTimerHandle,
        this,
        &ARandomMapGenerator::CheckToRandomMapComplete,
        1.0f,
        true
    );
}

void ARandomMapGenerator::CheckToRandomMapComplete()
{
    UE_LOG(LogTemp, Warning, TEXT("Running"));

    float TimeSeconds = GetWorld()->GetTimeSeconds();
    if (TimeSeconds >= RandomMapGeneratorDataAsset->MaxDungeonTime)
    {
        FString CurrentLevelName = GetWorld()->GetMapName();
        CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
        UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
    }

}


void ARandomMapGenerator::CheckForOverlap()
{
    AddOverlappingRoomsToList();


    if (!OverlappedList.IsEmpty())
    {
        OverlappedList.Empty();
        LatestRoom->Destroy();
        SpawnNextRoom();
    }
    else
    {
        OverlappedList.Empty();
        uint32 NewRoomAmount = GetRoomAmount() - 1;
        SetRoomAmount(NewRoomAmount);

        ExitsList.Remove(SelectedExitPoint);

        // TODO: Add to Door List

        TArray<USceneComponent*> OutChildren;
        LatestRoom->ExitsFolder->GetChildrenComponents(false, OutChildren);
        ExitsList.Append(OutChildren);

        if (GetRoomAmount() > 0)
        {
            InitializeItemSpawning();

            // TODO: Specal Room List 처리

            SpawnNextRoom();
        }
        else
        {
            UKismetSystemLibrary::K2_SetTimer(this, TEXT("CloseHoles"), 1.f, false);
            SetDungeonComplete(true);

            GetWorld()->GetTimerManager().ClearTimer(RandomMapTimerHandle);

            UWorld* World = GetWorld();
            if (!World)
            {
                UE_LOG(LogTemp, Warning, TEXT("World is not valid."));
                return;
            }

            AActor* FoundActor = UGameplayStatics::GetActorOfClass(World, ARandomItemSpawner::StaticClass());
            if (FoundActor)
            {
                ARandomItemSpawner* RandomItemSpawner = Cast<ARandomItemSpawner>(FoundActor);
                RandomItemSpawner->SpawnItemRandomly();
            }

            ABasicGameState* GameState = Cast<ABasicGameState>(UGameplayStatics::GetGameState(GetWorld()));
            if (!GameState)
            {
                ensure(false);
                return;
            }
            GameState->OnRandomMapCompleted();
            
        }
    }
}

void ARandomMapGenerator::AddOverlappingRoomsToList()
{
    TArray<USceneComponent*> OutChildren;
    LatestRoom->OverlapFolder->GetChildrenComponents(false, OutChildren);

    for (USceneComponent* Child : OutChildren)
    {
        UBoxComponent* ChildBox = Cast<UBoxComponent>(Child);
        if (ChildBox)
        {
            TArray<UPrimitiveComponent*> OutOverlappingComponents;
            ChildBox->GetOverlappingComponents(OutOverlappingComponents);

            OverlappedList.Append(OutOverlappingComponents);
        }
    }
}

void ARandomMapGenerator::CloseHoles()
{
    // TODO: Close Holes
    for (USceneComponent* Exit : ExitsList)
    {
        if (UWorld* World = GetWorld())
        {
            FTransform SelectedTransform = Exit->K2_GetComponentToWorld();
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            //TSubclassOf<AMasterRoom> RoomClass = FUtils::GetRandomElementFromArray(RandomMapGeneratorDataAsset->RoomList);

            AActor* SpawnedActor = World->SpawnActor<AActor>(RandomMapGeneratorDataAsset->WallClass, SelectedTransform, SpawnParams);
        }
    }
}

void ARandomMapGenerator::SetSeed()
{
    if (RandomMapGeneratorDataAsset->Seed == -1)
    {
        UKismetMathLibrary::SeedRandomStream(Stream);
    }
    else
    {
        UKismetMathLibrary::SetRandomStreamSeed(Stream, RandomMapGeneratorDataAsset->Seed);
    }
}

void ARandomMapGenerator::InitializeItemSpawning()
{
    TArray<USceneComponent*> OutChildren;
    LatestRoom->SpawnFolder->GetChildrenComponents(false, OutChildren);

    for(auto Child: OutChildren)
    {
        UBoxComponent* Box = Cast<UBoxComponent>(Child);
        if(Box)
        {
            SpawnBox = Box;
            SpawnItems();
        }
        else
        {
	        ensure(false);
        }
    }

}

void ARandomMapGenerator::SpawnItems()
{
    // 레벨에 배치된 아이템 수가 상한선을 넘는다면, 더 이상 배치하지 않기
    ABasicGameState* GameState = Cast<ABasicGameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (!GameState)
    {
        ensure(false);
        return;
    }
    if(GameState->SpawnedItems.Num() >= GameState->MaxItemAmount)
    {
        return;
    }

    // 각 방마다 랜덤한 숫자를 정해 그 수만큼 아이템을 스폰하기
   uint32 MinItemAmount = RandomMapGeneratorDataAsset->MinItemAMount;
   uint32 MaxItemAmount = RandomMapGeneratorDataAsset->MaxItemAmount;

   uint32 ItemAmount = UKismetMathLibrary::RandomIntegerInRange(MinItemAmount, MaxItemAmount);

    for(uint32 i = 0; i < ItemAmount; ++i)
    {
        // 박스에서 아이템이 스폰될 랜덤한 위치를 가져오기
        GetRandomPointsFromSpawnBox();

        // 실제 아이템을 스폰하는 코드
        FVector SpawnLocation = RandomBoxLocation;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        //TSubclassOf<AMasterRoom> ItemClass = FUtils::RandomArrayItemFromStream(RandomMapGeneratorDataAsset->ItemList, Stream);
        TSubclassOf<AItemBase> ItemClass = FUtils::GetRandomElementFromArray(RandomMapGeneratorDataAsset->ItemList);

    	if(!ItemClass)
        {
            ensure(false);
            return;
        }

        if (GameState->SpawnedItems.Num() >= GameState->MaxItemAmount)  // 스폰 직전에 스폰 상한선 넘었는지 다시 확인
        {
            return;
        }

        AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);

        // 스폰된 아이템을 게임 스테이트에 추가
        AItemBase* SpawnedItem = Cast<AItemBase>(SpawnedActor);

        if(!SpawnedItem)
        {
	        ensure(false);
            return;
        }
        GameState->SpawnedItems.Add(SpawnedItem, false);

        //UE_LOG(LogTemp, Warning, TEXT("%s is Spawned."), *SpawnedItem->GetItemTableRow()->DisplayName.ToString());
    }
}

void ARandomMapGenerator::GetRandomPointsFromSpawnBox(int32 MaxAttempts)
{
	// 박스 내의 랜덤한 위치를 구하기
    if (MaxAttempts <= 0) return; // 시도가 너무 많으면 종료(무한루프 방지)
    FVector BoxCenter = SpawnBox->GetComponentLocation();
    FVector BoxScale = SpawnBox->GetScaledBoxExtent();

    double NewX = UKismetMathLibrary::RandomFloatInRange(-BoxScale.X, BoxScale.X);
    double NewY = UKismetMathLibrary::RandomFloatInRange(-BoxScale.Y, BoxScale.Y);
    FVector NewVector = UKismetMathLibrary::MakeVector(NewX, NewY, 20.0);
    RandomBoxLocation = BoxCenter + NewVector;

    // 라인 트레이스를 통해 스폰되는 아이템을 바닥으로 붙이기
    FVector EndLocation = RandomBoxLocation - FVector(0.0, 0.0, 1000.0);
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, RandomBoxLocation, EndLocation, ECC_Visibility, CollisionParams);

    DrawDebugLine(GetWorld(), RandomBoxLocation, EndLocation, FColor::Green, false, INFINITY, 0, 1.0f);

	// 만약 완전히 공중이라면 랜덤한 위치를 다시 구함
    if (!bHit)
    {
        GetRandomPointsFromSpawnBox(MaxAttempts - 1); // 시도 횟수 감소
    }
    else
    {
        RandomBoxLocation = HitResult.Location;
    }
}

