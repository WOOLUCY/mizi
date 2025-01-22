// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SpiderEgg.h"
#include "PlayerCharacter/BasicCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


// Sets default values
ASpiderEgg::ASpiderEgg()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCanEverAffectNavigation(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Enemy/Spider/Asset/SM_Egg.SM_Egg'"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	}
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Spider Egg's Mesh is not found!"));
    }

	// Box Collision
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	// Navigation Invoker Component
	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
	NavigationInvoker->SetGenerationRadii(3000.0f, 5000.0f);

}

// Called when the game starts or when spawned
void ASpiderEgg::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpiderEgg::OnBeginOverlap);
	
}

// Called every frame
void ASpiderEgg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpiderEgg::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABasicCharacter* Player = Cast<ABasicCharacter>(OtherActor);
	if (!Player) { return; }

	// TODO: 알이 밟히는 소리 처리
	// Sound Attenuation 사용, 경로 하드 코딩

    AActor* InstigatorActor = GetInstigator();
    if (!InstigatorActor) { return; }

    // Instigator가 AIController인 경우 BlackboardComponent를 가져옴
    AAIController* AIController = Cast<AAIController>(InstigatorActor);
    if (AIController)
    {
        UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
        if (BlackboardComponent)
        {
            BlackboardComponent->SetValueAsBool(FName("TargetActor"), true);
			BlackboardComponent->SetValueAsVector(FName("SteppedLocation"), GetActorLocation());
        }
    }
}

