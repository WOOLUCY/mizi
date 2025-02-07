// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/Rifle.h"

#include "Components/DecalComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter/BasicCharacter.h"
#include "UI/BasicHUD.h"

ARifle::ARifle()
{
	ItemName = TEXT("Rifle");
}

void ARifle::OnUsed()
{
	Fire();
}

void ARifle::Fire()
{
	if (CurBulletAmount <= 0) return;
	if (!bCanFire) return;

	bCanFire = false;
	CurBulletAmount--;

	// Play Animation Montage
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	if (!Character->GetMovementComponent()->IsCrouching())
	{
		Character->PlayAnimMontage(Character->GetCharacterData()->RiffleFireMontage);
	}

	// Camera Shake
	if (GunTableRow->FireCameraShake)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), GunTableRow->FireCameraShake, GetActorLocation(), 0.f, 500.f);
	}

	// Spawn Fire Effect
	TArray<FName> RowNames = EffectTableRowHandle.DataTable->GetRowNames();

	if (EffectTableRowHandle.IsNull()) { return; }
	FEffectTableRow* Data = EffectTableRowHandle.GetRow<FEffectTableRow>(TEXT("Fire"));

	if (!Data) { ensure(false); }
	const FTransform EffectTransform = StaticMeshComponent->GetSocketTransform("MuzzleSocket");
	StaticMeshComponent->GetWorld()->GetSubsystem<UActorPoolSubsystem>()->SpawnEffect(EffectTransform, EffectTableRowHandle);

	// Widget Animation
	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	HUD->GetStatusWidget()->PlayAnimation(HUD->GetStatusWidget()->FireGun);

	if (HUD)
	{
		HUD->GetStatusWidget()->UpdateBulletText();
	}

	// Line Trace
	FVector StartLocation = Character->GetCameraWorldLocation();
	FVector EndLocation = Character->GetCameraForwardVector() * GunTableRow->Range + StartLocation;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 2.f, 0, 1.0f);

	if (!bHit) return;

	// Spawn Hit Emitter
	FTransform NewTransform;
	NewTransform.SetLocation(HitResult.ImpactPoint);

	FRotator Rotation = UKismetMathLibrary::Conv_VectorToRotator(HitResult.ImpactNormal);
	Rotation.Roll += FMath::RandRange(-180.0f, 180.0f);
	NewTransform.SetRotation(Rotation.Quaternion());

	int32 RandInt = UKismetMathLibrary::RandomIntegerInRange(1, 3);
	float RandFloat = RandInt / 100.0f;
	FVector Scale = UKismetMathLibrary::Conv_FloatToVector(RandFloat);
	NewTransform.SetScale3D(Scale);

	GetWorld()->GetSubsystem<UActorPoolSubsystem>()->SpawnHitEffectWithDecal(NewTransform,GunTableRow->HitEffectTableRowHandle);
	
	// Take Damage
	AActor* HitActor = HitResult.GetActor();
	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("💥 Bullet Hit: %s"), *HitActor->GetName());

		UGameplayStatics::ApplyPointDamage(
			HitActor,									
			GunTableRow->Damage,
			EndLocation - StartLocation,			
			HitResult,								
			UGameplayStatics::GetPlayerController(GetWorld(), 0),                  
			this,									
			UDamageType::StaticClass()					
		);
	}

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("SetCanFire"), GunTableRow->FireSpeed, false);
}
