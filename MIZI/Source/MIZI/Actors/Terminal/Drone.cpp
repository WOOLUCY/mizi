// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Terminal/Drone.h"

#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/MapErrors.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Drone = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Drone"));
	Drone->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Box"));
	Box->SetupAttachment(Drone, TEXT("BoxSocket"));
	//Box->SetRelativeLocation(FVector(0.0, -1486.0, -7262.0));
	//Box->SetRelativeRotation(FRotator(180.0, 0, 0));
	//Box->SetRelativeScale3D(FVector(100.0, 100.0, 100.0));

	ItemSpawnPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemSpawnPoint"));
	ItemSpawnPoint->SetupAttachment(RootComponent);
	ItemSpawnPoint->ShapeColor = FColor::Blue;
	ItemSpawnPoint->SetLineThickness(2.f);

}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("Activate"), 15.0f, false);
}

void ADrone::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsActive)	return;

	bIsRising ? OnRise() : OnFall();
}

void ADrone::OnFall()
{
	FindFloor();

	if(GetActorLocation().Z > StopLocation.Z)
	{
		AddActorLocalOffset(FVector(0.0, 0.0, UGameplayStatics::GetTimeSeconds(GetWorld()) * MoveSpeed * -1.0f));
		return;
	}

	if(!bIsArrived)
	{
		bIsArrived = true;
		OnArrive();
	}
}

void ADrone::OnArrive()
{
	// 드론 착륙 몽타주 재생
	if(!DroneMontage)
	{
		ensure(false);
		return;
	}

	UAnimInstance* AnimInstance = Drone->GetAnimInstance();
	AnimInstance->OnMontageEnded.AddDynamic(this, &ADrone::OnMontageEnded);
	AnimInstance->Montage_Play(DroneMontage);

	// 박스 오픈 애니메이션 재생
	if(!BoxOpen)
	{
		ensure(false);
		return;;
	}

	Box->PlayAnimation(BoxOpen, false);

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("SpawnItems"), 1.0f, false);
}

void ADrone::OnRise()
{
	if (GetActorLocation().Z > StopLocation.Z + 10000.0f)
	{
		Destroy();
	}

	AddActorLocalOffset(FVector(0.0, 0.0, UGameplayStatics::GetTimeSeconds(GetWorld()) * MoveSpeed / 5.0f));

}



void ADrone::FindFloor()
{
	// 라인 트레이스를 통해 착지 지점 확인
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = GetActorLocation() - FVector(0.0, 0.0, 5000.0);
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	if (!bHit)
	{
		return;
	}

	StopLocation = HitResult.Location + FVector(0.0, 0.0, 300.0);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, INFINITY, 0, 1.0f);
}

void ADrone::SpawnItems()
{
	for (auto Iter : OrderList)
	{
		for(int i = 0; i < Iter.Value; ++i)
		{
			// 박스 내의 랜덤한 위치
			FVector BoxCenter = ItemSpawnPoint->GetComponentLocation();
			FVector BoxScale = ItemSpawnPoint->GetScaledBoxExtent();

			float NewX = UKismetMathLibrary::RandomFloatInRange(-BoxScale.X, BoxScale.X);
			float NewY = UKismetMathLibrary::RandomFloatInRange(-BoxScale.Y, BoxScale.Y);
			float NewZ = UKismetMathLibrary::RandomFloatInRange(-BoxScale.Z, BoxScale.Z);
			FVector NewVector = FVector(NewX, NewY, NewZ);
			FVector RandomBoxLocation = BoxCenter + NewVector;

			// 아이템을 스폰
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FTransform SpawnTransform(ItemSpawnPoint->GetComponentRotation(), RandomBoxLocation);

			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(Iter.Key, SpawnTransform, SpawnParams);
			if (!SpawnedActor) return;
			AItemBase* Item = Cast<AItemBase>(SpawnedActor);

			// 스폰한 아이템에 물리 시뮬을 적용하고, 스폰 목록에 추가
			if(Item)
			{
				Item->StaticMeshComponent->SetSimulatePhysics(true);
				SpawnedItems.Add(Item);
			}
		}
	}

	// 일단 모두 스폰하고 2초 뒤에 다시 스폰
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("ReSpawnItemsWithoutPhysics"), 2.0f, false);

}

void ADrone::ReSpawnItemsWithoutPhysics()
{
	for(auto Iter : SpawnedItems)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* RespawnedActor = GetWorld()->SpawnActor<AActor>(Iter->GetClass(), Iter->StaticMeshComponent->GetComponentToWorld(), SpawnParams);

		Iter->Destroy();
	}
}

void ADrone::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsRising = true;
}

void ADrone::Activate()
{
	bIsActive = true;

	if (!DroneSound)
	{
		ensure(false);
		return;
	}

	// TODO: Audio Component로 변경할 것
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), DroneSound, GetActorLocation());

}

