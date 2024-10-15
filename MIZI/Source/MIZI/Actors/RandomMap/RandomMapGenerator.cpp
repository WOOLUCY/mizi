// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RandomMap/RandomMapGenerator.h"
#include "Engine/World.h"
#include "Actors/RandomMap/MasterRoom.h"
#include "Misc/Utils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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

}

// Called when the game starts or when spawned
void ARandomMapGenerator::BeginPlay()
{
    Super::BeginPlay();

    SetRoomAmount(RandomMapGeneratorDataAsset->MaxRoomAmount);
    //SetRoomAmount(10);

    SpawnStartRoom();
    StartRandomMapTimer();
    SpawnNextRoom();
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
        SelectedExitPoint = FUtils::GetRandomElementFromArray(ExitsList);

        FTransform SelectedTransform = SelectedExitPoint->K2_GetComponentToWorld();
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        TSubclassOf<AMasterRoom> RoomClass = FUtils::GetRandomElementFromArray(RandomMapGeneratorDataAsset->RoomList);
        
        AActor* SpawnedActor = World->SpawnActor<AActor>(RoomClass, SelectedTransform, SpawnParams);
        LatestRoom = Cast<AMasterRoom>(SpawnedActor);
    }
}


void ARandomMapGenerator::StartRandomMapTimer()
{

}

void ARandomMapGenerator::CheckToRandomMapComplete()
{
}


void ARandomMapGenerator::CheckForOverlap()
{
}

void ARandomMapGenerator::AddOverlappingRoomsToList()
{
}

void ARandomMapGenerator::CloseHoles()
{
}

