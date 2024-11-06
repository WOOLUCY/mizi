// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item/Rifle.h"

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
	// 총알이 충분한지 확인
	if (CurBulletAmount <= 0) return;

	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	if(!Character->GetMovementComponent()->IsCrouching())
	{
		Character->PlayAnimMontage(Character->GetCharacterData()->RiffleFireMontage);
	}

	CurBulletAmount--;

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("FireBullet"), 0.2f, false);

	Super::OnUsed();
}

void ARifle::OnEquiped()
{
	Super::OnEquiped();

	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	// 장착 애니메이션 몽타주 재생
	Character->PlayAnimMontage(Character->GetCharacterData()->RiffleEquipMontage);

	// 애니메이션 변경
	Character->GetMesh()->SetAnimClass(Character->GetCharacterData()->GunAnimClass);

	// 위젯 표시
	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	if (HUD)
	{
		HUD->GetStatusWidget()->RevealBulletWidget();
	}
}

void ARifle::OnUnEquiped()
{
	Super::OnUnEquiped();

	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	// 장착 해제 애니메이션 몽타주 재생
	Character->PlayAnimMontage(Character->GetCharacterData()->RiffleEquipMontage);

	// 애니메이션 변경
	Character->GetMesh()->SetAnimClass(Character->GetCharacterData()->AnimClass);

	// 위젯 숨김
	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	if (HUD)
	{
		HUD->GetStatusWidget()->HideBulletWidget();
	}
}

void ARifle::FireBullet()
{
	// Camera Shake
	if (FireCameraShake)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), FireCameraShake, GetActorLocation(), 0.f, 500.f);
	}

	// Spawn Emitter
	if (FireEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(FireEffect, StaticMeshComponent, FName("MuzzleSocket"),FVector(ForceInit), FRotator::ZeroRotator, FVector(0.05));
	}

	// Play Sound
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}

	// Line Trace
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(!Character)
	{
		ensure(false);
		return;
	}

	FVector StartLocation = Character->GetCameraWorldLocation();
	FVector EndLocation = Character->GetCameraForwardVector() * 3000.f + StartLocation;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, INFINITY, 0, 1.0f);

	if (!bHit) return;

	// Hit Effect
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.Location, FRotator::ZeroRotator, FVector(0.1, 0.1, 0.1));
	}

	// Decal
	if (BulletMarkDecal)
	{
		FVector Scale = UKismetMathLibrary::Conv_IntToVector(UKismetMathLibrary::RandomIntegerInRange(3, 7));

		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletMarkDecal,
			Scale, HitResult.Location, UKismetMathLibrary::Conv_VectorToRotator(HitResult.ImpactNormal));

		Decal->SetFadeScreenSize(0.f);	// 멀리 가면 데칼이 화면에 보이지 않는 것을 방지
	}

	TSubclassOf<UDamageType> DamageTypeClass;
	UGameplayStatics::ApplyDamage(HitResult.GetActor(), RiffleDamage, nullptr, GetOwner(), DamageTypeClass);
}
