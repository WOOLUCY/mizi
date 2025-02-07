// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/GunBase.h"
#include "PlayerCharacter/BasicCharacter.h"
#include "UI/BasicHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Weapon/BulletBase.h"



AGunBase::AGunBase()
{
	ItemName = TEXT("SMG");
}

void AGunBase::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	// Gun Data Table
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FGunTableRow* Data = DataTableRowHandle.GetRow<FGunTableRow>(ItemName);
	if (!Data) { ensure(false); return; }

	GunTableRow = Data;
	ItemTableRow = Data;

	StaticMeshComponent->SetStaticMesh(Data->StaticMesh);
	Weight = GunTableRow->Weight;

	MaxBulletAmount = GunTableRow->MaxBulletAmount;
	CurBulletAmount = GunTableRow->CurBulletAmount;
	

}

void AGunBase::OnEquiped()
{
	if (GunTableRow->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunTableRow->EquipSound, GetActorLocation());
	}

	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	Character->GetMesh()->SetAnimClass(Character->GetCharacterData()->GunAnimClass);

	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	if (HUD)
	{
		HUD->GetStatusWidget()->RevealBulletWidget();
	}
}

void AGunBase::OnUnEquiped()
{
	//if (GunTableRow->UnequipSound)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunTableRow->UnequipSound, GetActorLocation());
	//}

	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	Character->GetMesh()->SetAnimClass(Character->GetCharacterData()->AnimClass);

	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	if (HUD)
	{
		HUD->GetStatusWidget()->HideBulletWidget();
	}
}

void AGunBase::OnUsed()
{
	Fire();
}

void AGunBase::Fire()
{
	//if (CurBulletAmount <= 0) return;
	if (!bCanFire) return;

	bCanFire = false;
	CurBulletAmount--;

	// 	Play Animation
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

	// Spawn Bullet
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector MuzzleLocation = StaticMeshComponent->GetSocketLocation(FName("MuzzleSocket"));
	FVector ShootDirection = CameraRotation.Vector();

	FTransform BulletTransform;
	BulletTransform.SetLocation(MuzzleLocation);
	BulletTransform.SetRotation(ShootDirection.Rotation().Quaternion());

	ABulletBase* Bullet = GetWorld()->SpawnActorDeferred<ABulletBase>(
		ABulletBase::StaticClass(), BulletTransform, this, this->GetInstigator(), ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);

	if (Bullet)
	{
		Bullet->SetOwner(this);
		Bullet->FinishSpawning(BulletTransform);
	}

	// Camera Shake
	if (GunTableRow->FireCameraShake)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), GunTableRow->FireCameraShake, GetActorLocation(), 0.f, 500.f);
	}

	// Update Bullet Widget
	ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	HUD->GetStatusWidget()->PlayAnimation(HUD->GetStatusWidget()->FireGun);

	if (HUD)
	{
		HUD->GetStatusWidget()->UpdateBulletText();
	}

	// Spawn Fire Effect
	TArray<FName> RowNames = EffectTableRowHandle.DataTable->GetRowNames();

	if (EffectTableRowHandle.IsNull()) { return; }
	FEffectTableRow* Data = EffectTableRowHandle.GetRow<FEffectTableRow>(TEXT("Fire"));

	if (!Data) { ensure(false); return; }
	const FTransform EffectTransform = StaticMeshComponent->GetSocketTransform("MuzzleSocket");
	StaticMeshComponent->GetWorld()->GetSubsystem<UActorPoolSubsystem>()->SpawnEffect(EffectTransform, EffectTableRowHandle);

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("SetCanFire"), GunTableRow->FireSpeed, false);
}

void AGunBase::SetCanFire()
{
	bCanFire = true;
}

