// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/BasicCharacter.h"

#include "Actors/Gimmick/GimmickBase.h"
#include "Camera/CameraActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/BasicHUD.h"
#include "Materials/MaterialParameterCollection.h"


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
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	{
		SpringArm->SetupAttachment(GetMesh());
		SpringArm->ProbeSize = 5.0;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bEnableCameraRotationLag = true;
		SpringArm->bInheritRoll = false;
		//SpringArm->SetMinMaxTargetArmLength(200.f, SpringArm->GetMaxTargetArmLength());
	}
	ThirdPersonCamera->SetupAttachment(SpringArm);
	ThirdPersonCamera->SetAutoActivate(false);

	//bUseControllerRotationYaw = false;

	//const FRotator Rotation = FRotator(0., 90.0, 0.);
	//const FVector Translation = FVector(0., 0., 90.0);
	//FTransform SpringArmTransform = FTransform(Rotation, Translation, FVector::OneVector);
	//SpringArm->SetRelativeTransform(SpringArmTransform);

	/* Timeline Component */
	ScanTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScanTimeline"));

	ParallaxMPC = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Script/Engine.MaterialParameterCollection'/Game/Character/Widget/MPC_Parallax.MPC_Parallax'"));
	if (!ParallaxMPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parallax HUD Material Collection not found!"));
		return;
	}

	// Widget Interaction
	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteraction->SetupAttachment(FirstPersonCamera);

}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(CharacterData->ScanTimeline)
	{
		FOnTimelineFloat ProgressUpdate;	// Callback Function for Curve
		ProgressUpdate.BindUFunction(this, FName("ScanRadiusUpdate"));	// Binding
		ScanTimeLine.AddInterpFloat(CharacterData->ScanTimeline, ProgressUpdate);

		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindUFunction(this, FName("OnScanFinished"));
		ScanTimeLine.SetTimelineFinishedFunc(TimelineFinishedEvent);
	}

	// Player State
	ABasicPlayerState* BasicPlayerState = Cast<ABasicPlayerState>(GetPlayerState());
	if(BasicPlayerState)
	{
		Status = BasicPlayerState;
	}
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
		Movement->MaxWalkSpeed = CharacterData->WalkMaxSpeed;

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

	// Scan
	//{
	//	ScanSound = CharacterData->ScanSound;
	//	ScanCurveFloat = CharacterData->ScanTimeline;
	//	ScanCollection = CharacterData->ScanCollection;
	//	ScanRadiusMultiplier = CharacterData->ScanRadiusMultiplier;
	//}
}

float ABasicCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	// Decrease HP
	int32 NewHealth = Status->GetCurHealth() - DamageAmount;
	UKismetMathLibrary::Clamp(NewHealth, 0, Status->GetMaxHealth());
	Status->SetCurHealth(NewHealth);

	if (NewHealth <= 0)
	{
		OnDie();
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	// HUD
	ABasicHUD* BasicHUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (!BasicHUD)
	{
		ensure(false);
	}
	BasicHUD->GetStatusWidget()->OnDamaged();

	// Camera Shake
	int NewDamage = UKismetMathLibrary::FTrunc(DamageAmount) / 20;
	switch (NewDamage)
	{
	case 1:
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CharacterData->DamageCameraShake01, GetActorLocation(), 0.0, 1000.0);
		break;
	case 2:
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CharacterData->DamageCameraShake02, GetActorLocation(), 0.0, 1000.0);
		break;
	case 3:
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CharacterData->DamageCameraShake03, GetActorLocation(), 0.0, 1000.0);
		break;
	case 4:
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CharacterData->DamageCameraShake04, GetActorLocation(), 0.0, 1000.0);
		break;
	case 5:
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CharacterData->DamageCameraShake05, GetActorLocation(), 0.0, 1000.0);
		break;
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ChangeControllerRotationYaw();

	ScanTimeLine.TickTimeline(DeltaTime);	// Timeline에 DeltaTime 전달

	SetParallaxHUDOffset();
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

	// Play Sound 
	USoundBase* Sound = CharacterData->ScanSound;
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);

	// Timeline
	ScanTimeLine.PlayFromStart();

	//for (AItemBase* Item : ScannedItems)
	//{
	//	Item->OnScanned();
	//}
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

void ABasicCharacter::OnPickUpItem()
{
	if (OwningItems.Contains(CurInventoryIndex)) { return; }

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AItemBase::StaticClass());


	for(AActor* Actor : OverlappingActors)
	{
		AItemBase* Item = Cast<AItemBase>(Actor);
		OverlappedItem = Item;
		OverlappedItem->SetActorEnableCollision(false);
		OverlappedItem->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			OverlappedItem->GetItemTableRow()->EquipSocketName);

		OwningItems.Add(CurInventoryIndex, OverlappedItem);

		OnInventoryChanged();

		Item->OnPicked();
		
		break;
	}
}

void ABasicCharacter::OnDropItem()
{
	if(OwningItems.Contains(CurInventoryIndex))
	{
		// 트레이스 시작 위치와 끝 위치 설정
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation - FVector(0, 0, 500);

		// 히트 결과를 저장할 변수
		FHitResult OutHit;

		// 트레이스 실행
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			OutHit,                             // 히트 결과 저장할 FHitResult
			StartLocation,                      // 트레이스 시작 위치
			EndLocation,                        // 트레이스 끝 위치
			ECC_Visibility,                     // 트레이스할 충돌 채널 (예: ECC_Visibility)
			FCollisionQueryParams()             // 추가 옵션 설정 (기본값으로 사용)
		);

		// 히트된 경우 처리
		if (bHit)
		{
			AActor* HitActor = OutHit.GetActor();
			if (HitActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());

				// 히트된 위치 시각화 (디버그)
				//DrawDebugLine(
				//	GetWorld(),
				//	StartLocation,
				//	EndLocation,
				//	FColor::Red,
				//	false, 5.0f, 0, 1.0f
				//);

				//DrawDebugSphere(
				//	GetWorld(),
				//	OutHit.ImpactPoint,
				//	10.0f,
				//	12,
				//	FColor::Green,
				//	false, 5.0f
				//);

				FVector NewLocation = OutHit.Location + FVector(100.0, 0.0, 0.0);
				const FRotator Rotation = FRotator(0., 90.0, 0.);
				const FVector Translation = FVector(0., 0., 90.0);

				auto item = OwningItems.Find(CurInventoryIndex);
				(*item)->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				(*item)->SetActorTransform(FTransform(NewLocation));

				(*item)->SetActorEnableCollision(true);
				(*item)->SetActorHiddenInGame(false);

				OwningItems.Remove(CurInventoryIndex);

				// TODO: On Inventory Change 호출
				OnInventoryChanged();

				(*item)->OnDropped();
				
			}
		}
	}
}

void ABasicCharacter::OnInventoryChanged()
{
	OnEquipChanged();

	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	// 전체 무게 수정
	Weight = 0;
	for(auto Iter : OwningItems)
	{
		Weight += Iter.Value->GetWeight();
	}
	CalculatePlayerSpeed();

	if (!HUD)
	{
		ensure(false);
		return;
	}
	//HUD->GetInventoryWidget()->OnInventoryChanged();
	HUD->GetStatusWidget()->OnInventoryChanged();
	HUD->GetStatusWidget()->UpdateWeightText();
}

void ABasicCharacter::OnInventoryIndexChanged(float Value)
{
	PrevInventoryIndex = CurInventoryIndex;

	float AddIndex = CurInventoryIndex + Value;
	CurInventoryIndex = (int)AddIndex % MaxInventoryIndex;

	OnEquipChanged();

	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	if(!HUD)
	{
		ensure(false);
		return;
	}
	//HUD->GetInventoryWidget()->OnInventoryIndexChanged();
	HUD->GetStatusWidget()->OnInventoryIndexChanged();
}

void ABasicCharacter::OnEquipChanged()
{
	auto PrevEquippedItem = OwningItems.Find(PrevInventoryIndex);
	if(PrevEquippedItem)
	{
		(*PrevEquippedItem)->SetActorHiddenInGame(true);
		(*PrevEquippedItem)->OnUnEquiped();
	}

	auto CurEquippedItem = OwningItems.Find(CurInventoryIndex);
	if (CurEquippedItem)
	{
		(*CurEquippedItem)->SetActorHiddenInGame(false);
		(*CurEquippedItem)->OnEquiped();
	}
}

void ABasicCharacter::StartSprinting()
{
	float NewSpeed = CalculatePlayerSpeed() + 200.0f;
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	SetIsSprinting(true);
	DrainStamina();
}

void ABasicCharacter::StopSprinting()
{
	CalculatePlayerSpeed();
	SetIsSprinting(false);
	RegenStamina();
}

void ABasicCharacter::OnUseItem()
{
	auto EquippedItem = OwningItems.Find(CurInventoryIndex);
	if (!EquippedItem) return;
	(*EquippedItem)->OnUsed();
 }

void ABasicCharacter::OnUseItemTriggered()
{
	auto EquippedItem = OwningItems.Find(CurInventoryIndex);
	if (!EquippedItem) return;
	(*EquippedItem)->OnUsedTriggered();
}

void ABasicCharacter::OnTerminalPressed()
{
	if(bCanUseConsole)
	{
		WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
	}
	else
	{
		WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
	}
}

void ABasicCharacter::OnTerminalReleased()
{
	WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

float ABasicCharacter::OnSignAttack(TSet<AActor*> DamagedActors)
{
	GetMovementComponent()->StopMovementImmediately();

	TArray<UAnimMontage*> AttackMontages = CharacterData->SignAttackMontage;
	if (AttackMontages.IsEmpty())
	{
		ensure(false);
		return 0.f;
	}
	float AnimationDuration = PlayAnimMontage(AttackMontages[0]);

	TSubclassOf<UDamageType> DamageType;

	for(AActor* DamagedActor : DamagedActors)
	{
		float Damage = UGameplayStatics::ApplyDamage(DamagedActor, 20.f, nullptr, this, DamageType);
		UE_LOG(LogTemp, Log, TEXT("Player is attacking %s: %f"), *(DamagedActor->GetName()), Damage);
	}

	return AnimationDuration;
}


void ABasicCharacter::OnDie()
{
	ABasicPlayerState* BasicPlayerState = Cast<ABasicPlayerState>(GetPlayerState());
	if (!BasicPlayerState)
	{
		ensure(false);
	}

	BasicPlayerState->SetIsDead(true);

	ABasicHUD* BasicHUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (!BasicHUD)
	{
		ensure(false);
	}
	BasicHUD->GetStatusWidget()->SetVisibility(ESlateVisibility::Hidden);

	GetMesh()->SetSimulatePhysics(true);
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Camera
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* Cam = GetWorld()->SpawnActor<AActor>(ACameraActor::StaticClass(), GetActorTransform(), SpawnParams);

	ACameraActor* DeadCam = Cast<ACameraActor>(Cam);
	if (!DeadCam) return;

	//UKismetSystemLibrary::K2_SetTimer(this, TEXT("MoveToDeadCam"), 1.0f, false);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, DeadCam]()
		{
			MoveToDeadCam(DeadCam);
		}, 1.0f, false);
}

void ABasicCharacter::MoveToDeadCam(ACameraActor* Cam)
{
	FVector NewLocation = GetMesh()->GetSocketLocation(TEXT("FlashSocket")) + FVector(0.0, 0.0, 300.0);
	Cam->SetActorLocation(NewLocation);

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Cam->GetActorLocation(), GetMesh()->GetSocketLocation(TEXT("FlashSocket")));
	Cam->SetActorRotation(NewRotation);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cam, 1);

	// TODO: Play Sequence
}


void ABasicCharacter::DrainStamina()
{
	uint32 NewStamina = UKismetMathLibrary::Clamp((Status->GetCurStamina() - 1), 0, Status->GetMaxStamina());
	Status->SetCurStamina(NewStamina);

	if(NewStamina == 0)
	{
		StopSprinting();
	}
	else
	{
		if(GetIsSprinting())
		{
			UKismetSystemLibrary::K2_SetTimer(this, TEXT("DrainStamina"), CharacterData->StaminaDrainSpeed, false);
		}
	}

}

void ABasicCharacter::RegenStamina()
{
	uint32 NewStamina = UKismetMathLibrary::Clamp((Status->GetCurStamina() + 2), 0, Status->GetMaxStamina());
	Status->SetCurStamina(NewStamina);

	if (Status->GetMaxStamina() == Status->GetCurStamina())
	{
		return;
	}
	else
	{
		if (GetIsSprinting())
		{
			return;
		}
		else
		{
			UKismetSystemLibrary::K2_SetTimer(this, TEXT("RegenStamina"), CharacterData->StaminaRegenSpeed, false);
		}
	}

}

void ABasicCharacter::ScanRadiusUpdate(float Radius)
{
	if (CharacterData->ScanCollection)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), CharacterData->ScanCollection, TEXT("Radius"), Radius);


		/* Sphere Trace */
		// 시작 위치와 끝 위치 설정
		FVector StartLocation = FirstPersonCamera->K2_GetComponentLocation();
		FVector EndLocation = StartLocation + FVector(0, 0, 1); // 위쪽으로 1000 유닛

		// 구의 반지름 설정
		float SphereRadius = Radius * CharacterData->ScanRadiusMultiplier;

		// 검색할 오브젝트 타입 설정
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

		// 무시할 액터 설정
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this); // 현재 액터 무시

		// 히트 결과를 저장할 배열
		TArray<FHitResult> OutHits;

		// 디버그 옵션 설정
		EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;

		// SphereTraceMulti 실행
		bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			StartLocation,
			EndLocation,
			SphereRadius,
			ObjectTypes,
			false,           // Complex trace 여부
			ActorsToIgnore,
			DebugType,
			OutHits,
			true             // 히트 여부 반환
		);

		// 히트된 오브젝트들 처리
		if (!bHit || OutHits.IsEmpty())
		{
			return;
		}

		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor)
			{
				continue;
			}

			// 아이템 처리
			AItemBase* ScannedItem = Cast<AItemBase>(HitActor);
			if (ScannedItem && !ScannedItems.Contains(ScannedItem))
			{
				ScannedItems.Add(ScannedItem);
				ScannedItem->OnScanned();
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
			}

			// 기믹 처리
			AGimmickBase* ScannedGimmick = Cast<AGimmickBase>(HitActor);
			if (ScannedGimmick && !ScannedGimmicks.Contains(ScannedGimmick))
			{
				ScannedGimmicks.Add(ScannedGimmick);
				ScannedGimmick->OnScanned();
			}
		}

	}
}

void ABasicCharacter::OnScanFinished()
{
	ScannedItems.Empty();
	ScannedGimmicks.Empty();
}

void ABasicCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (!GetMovementComponent()->IsFalling()) return;

	float Velocity = GetVelocity().Z;
	Velocity *= -1.0f;

	if (Velocity <= FallDamageTolerance) return;
	FallDamage = Velocity / 13.0f;

	TSubclassOf<UDamageType> DamageType;
	float Damage = UGameplayStatics::ApplyDamage(this, FallDamage, nullptr, this, DamageType);
	UE_LOG(LogTemp, Warning, TEXT("Fall Damage: %f"), Damage);
}

void ABasicCharacter::SetParallaxHUDOffset()
{
	FRotator InitialRot = FirstPersonCamera->GetComponentRotation();

	float NewPitch = InitialRot.Pitch - CameraRotator.Pitch + PitchRate;
	NewPitch = UKismetMathLibrary::FClamp(NewPitch, -1.0 * MinMaxPitchRate, MinMaxPitchRate);
	PitchRate = NewPitch;

	float NewYaw = InitialRot.Yaw - CameraRotator.Yaw + YawRate;
	NewYaw = UKismetMathLibrary::FClamp(NewYaw, -1.0 * MinMaxYawRate, MinMaxYawRate);
	YawRate = NewYaw;

	CameraRotator = InitialRot;

	PitchParallaxOffset = UKismetMathLibrary::FInterpTo(PitchParallaxOffset, PitchRate,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed);
	YawParallaxOffset = UKismetMathLibrary::FInterpTo(YawParallaxOffset, YawRate,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed);
	FLinearColor ParallaxOffsetValue = FLinearColor(YawParallaxOffset, PitchParallaxOffset, 0.0f, 1.0f);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), ParallaxMPC, FName("ParallaxOffset"), ParallaxOffsetValue);

	PitchRate = UKismetMathLibrary::FInterpTo(PitchRate, 0.0, 
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed);
	YawRate = UKismetMathLibrary::FInterpTo(YawRate, 0.0,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed);
}

float ABasicCharacter::CalculatePlayerSpeed()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if(Weight <= CharacterData->WeightNoEffect)
	{
		// 10kg 이하일 때는 기본 속도 유지
		Movement->MaxWalkSpeed = CharacterData->WalkMaxSpeed;
		return CharacterData->WalkMaxSpeed;
	}
	else if(Weight <= CharacterData->WeightThreshold)
	{
		// 10kg 이상 40kg 이하일 때는 속도가 기본 속도에서 점차 감소
		float Speed = CharacterData->WalkMaxSpeed - (CharacterData->WalkMaxSpeed - CharacterData->WalkMinSpeed) * (Weight - CharacterData->WeightNoEffect) / (CharacterData->WeightThreshold - CharacterData->WeightNoEffect);
		Movement->MaxWalkSpeed = Speed;
		return Speed;
	}
	else
	{
		// 40kg 이상일 때는 최소 속도 400으로 고정
		Movement->MaxWalkSpeed = CharacterData->WalkMinSpeed;
		return CharacterData->WalkMinSpeed;
	}
}

void ABasicCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	RecalculateBaseEyeHeight();
	FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
	MeshRelativeLocation.Z = CharacterData->MeshTransform.GetLocation().Z + HalfHeightAdjust;
	BaseTranslationOffset.Z = MeshRelativeLocation.Z;

	K2_OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ABasicCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	RecalculateBaseEyeHeight();

	FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
	MeshRelativeLocation.Z = CharacterData->MeshTransform.GetLocation().Z;
	BaseTranslationOffset.Z = MeshRelativeLocation.Z;

	K2_OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

