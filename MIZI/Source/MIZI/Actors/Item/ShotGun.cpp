// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item/ShotGun.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/BasicCharacter.h"
#include "UI/BasicHUD.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"

AShotGun::AShotGun()
{
	ItemName = TEXT("ShotGun");

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	/*MaxBulletAMount = 2;*/
	Damage = 100.0f;
    Range = 1000.0f;
}

void AShotGun::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FItemTableRow* Data = DataTableRowHandle.GetRow<FItemTableRow>(ItemName);
	if (!Data) { ensure(false); return; }

	ItemTableRow = Data;
	SkeletalMeshComponent->SetSkeletalMesh(Data->SkeletalMesh);

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetVisibility(false);
}

void AShotGun::OnUsed()
{
    if (bIsFiring) return;
    bIsFiring = true;

    //UKismetSystemLibrary::K2_SetTimer(this, TEXT("FireBullet"), 0.1f, false);

    Super::OnUsed();
}

void AShotGun::FireBullet()
{
    // Camera Shake
    if (FireCameraShake)
    {
        UGameplayStatics::PlayWorldCameraShake(GetWorld(), FireCameraShake, GetActorLocation(), 0.f, 500.f);
    }

    // Spawn Emitter
    if (FireEffect)
    {
        UGameplayStatics::SpawnEmitterAttached(FireEffect, SkeletalMeshComponent, FName("MuzzleSocket"), FVector(ForceInit), FRotator::ZeroRotator, FVector(0.05));
    }

    // Play Sound
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
    }

    ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Character)
    {
        ensure(false);
        return;
    }

    // Widget Animation
    ABasicHUD* HUD = Cast<ABasicHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
    HUD->GetStatusWidget()->PlayAnimation(HUD->GetStatusWidget()->FireGun);

    // Line Trace
    FVector StartLocation = Character->GetCameraWorldLocation();
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);


    for (int32 i = 0; i < 8; ++i)
    {
        FVector ShotDirection = FMath::VRandCone(Character->GetCameraForwardVector(), FMath::DegreesToRadians(10.0f));
        FVector EndLocation = StartLocation + (ShotDirection * Range); // Adjust range as needed

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 1.0f, 0, 1.0f);

        if (bHit)
        {
            // Hit Effect
            if (HitEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.Location, FRotator::ZeroRotator, FVector(0.1f, 0.1f, 0.1f));
            }

            // Decal
            if (BulletMarkDecal)
            {
                FVector Scale = UKismetMathLibrary::Conv_IntToVector(UKismetMathLibrary::RandomIntegerInRange(1, 5));

                FRotator RandomRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

                UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletMarkDecal,
                    Scale, HitResult.Location, UKismetMathLibrary::Conv_VectorToRotator(HitResult.ImpactNormal) + RandomRotation);

                Decal->SetFadeScreenSize(0.f);	// 멀리 가면 데칼이 화면에 보이지 않는 것을 방지
            }

            AActor* HitActor = HitResult.GetActor();
            if (HitActor)
            {
                // Apply damage
                UGameplayStatics::ApplyPointDamage(
                    HitActor,
                    Damage,
                    ShotDirection,
                    HitResult,
                    UGameplayStatics::GetPlayerController(GetWorld(), 0),
                    this,
                    UDamageType::StaticClass()
                );
            }
        }
    }

    UKismetSystemLibrary::K2_SetTimer(this, TEXT("SetFireState"), FireSound->GetDuration() + 0.1f, false);
}

void AShotGun::SetFireState()
{
    bIsFiring = false;
}

