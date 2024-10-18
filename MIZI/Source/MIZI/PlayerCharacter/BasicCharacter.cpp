// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/BasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABasicCharacter::ABasicCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(Super::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GetCapsuleComponent()->SetCollisionProfileName(CollisionProfileName::Player);

	SpringArm = CreateDefaultSubobject<USoftWheelSpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Weapon = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("Weapon"));
	{
		SpringArm->SetupAttachment(GetMesh());
		SpringArm->ProbeSize = 5.0;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritRoll = false;
		SpringArm->SetMinMaxTargetArmLength(200.f, SpringArm->GetMaxTargetArmLength());
	}
	Camera->SetupAttachment(SpringArm);
	//Weapon->SetupAttachment(GetMesh(), SocketName::Weapon);

	bUseControllerRotationYaw = false;

	//UCharacterMovementComponent* Movement = GetCharacterMovement();
	//Movement->bCanWalkOffLedges = false;
	const FRotator Rotation = FRotator(0., 90.0, 0.);
	const FVector Translation = FVector(0., 0., 90.0);
	FTransform SpringArmTransform = FTransform(Rotation, Translation, FVector::OneVector);
	SpringArm->SetRelativeTransform(SpringArmTransform);

}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasicCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
}

void ABasicCharacter::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FCharacterTableRow* Data = DataTableRowHandle.GetRow<FCharacterTableRow>(TEXT("BasicCharacter"));
	if (!Data) { ensure(false); return; }

	CharacterData = Data;
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		//Movement->RotationRate = CharacterData->RotationRate;
		Movement->bOrientRotationToMovement = true;
		Movement->GetNavAgentPropertiesRef().bCanCrouch = true;
		Movement->MaxWalkSpeed = CharacterData->MovementMaxSpeed;

		const float NewCapsuleHalfHeight = CharacterData->CollisionCapsuleHalfHeight * 0.5f;
		Movement->SetCrouchedHalfHeight(NewCapsuleHalfHeight);
	}
	{
		UCapsuleComponent* Capsule = GetCapsuleComponent();
		if (!FMath::IsNearlyEqual(Capsule->GetUnscaledCapsuleHalfHeight(), CharacterData->CollisionCapsuleHalfHeight))
		{
			Capsule->SetCapsuleHalfHeight(CharacterData->CollisionCapsuleHalfHeight, false);
		}
	}
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		SkeletalMeshComponent->SetSkeletalMesh(CharacterData->SkeletalMesh);
		SkeletalMeshComponent->SetRelativeTransform(CharacterData->MeshTransform);
		//SkeletalMeshComponent->SetAnimClass(CharacterData->AnimClass);
	}

}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

