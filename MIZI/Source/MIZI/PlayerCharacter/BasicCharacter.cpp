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

	/* First Person Camera */
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh());
	FirstPersonCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Head"));

	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->FieldOfView = 80.0;
	//FirstPersonCamera->SetAutoActivate(true);

	/* Third Person Camera */
	SpringArm = CreateDefaultSubobject<USoftWheelSpringArmComponent>(TEXT("SpringArm"));
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	{
		SpringArm->SetupAttachment(GetMesh());
		SpringArm->ProbeSize = 5.0;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bEnableCameraRotationLag = true;
		SpringArm->bInheritRoll = false;
		SpringArm->SetMinMaxTargetArmLength(200.f, SpringArm->GetMaxTargetArmLength());
	}
	ThirdPersonCamera->SetupAttachment(SpringArm);
	ThirdPersonCamera->SetAutoActivate(false);

	//bUseControllerRotationYaw = false;

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

		Movement->bUseControllerDesiredRotation = true;

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
		SkeletalMeshComponent->SetAnimClass(CharacterData->AnimClass);
	}

}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ChangeControllerRotationYaw();
}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicCharacter::ChangeControllerRotationYaw()
{
	FVector Velocity = GetVelocity();
	float Speed = Velocity.Size();

	if (Speed > 0.5f)
	{
		bUseControllerRotationYaw = true;
	}

	else
	{
		bUseControllerRotationYaw = false;
	}

}

void ABasicCharacter::OnScan()
{
	UE_LOG(LogTemp, Warning, TEXT("Scanning!"));

}

void ABasicCharacter::OnChangePerspective()
{
	if(bUseFirstPersonCamera)
	{
		// change perspective into TPS
		ThirdPersonCamera->SetActive(true);
		FirstPersonCamera->SetActive(false);
		bUseControllerRotationYaw = false;

		bUseFirstPersonCamera = false;
	}
	else
	{
		// change perspective into FPS
		ThirdPersonCamera->SetActive(false);
		FirstPersonCamera->SetActive(true);
		bUseFirstPersonCamera = false;

		bUseControllerRotationYaw = true;
		bUseFirstPersonCamera = true;
	}
}

