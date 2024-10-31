// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RandomMap/RandomMapGenerator.h"
#include "Engine/World.h"
#include "Actors/RandomMap/MasterRoom.h"
//#include "DataWrappers/ChaosVDQueryDataWrappers.h"
#include "RandomItemSpawner.h"
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

        TArray<USceneComponent*> OutChildren;
        LatestRoom->ExitsFolder->GetChildrenComponents(false, OutChildren);
        ExitsList.Append(OutChildren);

        if (GetRoomAmount() > 0)
        {
            SpawnNextRoom();
        }
        else
        {
            UKismetSystemLibrary::K2_SetTimer(this, TEXT("CloseHoles"), 1.f, false);
            SetDungeonComplete(true);

            GetWorld()->GetTimerManager().ClearTimer(RandomMapTimerHandle);
            UE_LOG(LogTemp, Warning, TEXT("Dungeon Complete"));

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

