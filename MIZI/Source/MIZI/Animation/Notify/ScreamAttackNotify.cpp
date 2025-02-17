// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/ScreamAttackNotify.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Enemy/EnemyBase.h"

void UScreamAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AttackTolerance = 600.f;

    if (ScreamEffect && MeshComp)
    {
        FVector Location = MeshComp->GetComponentLocation() + FVector(0, -60, 100);
        FRotator Rotation = MeshComp->GetComponentRotation();
        FVector Scale = FVector(1.0f, 1.0f, 1.0f);

        UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(), ScreamEffect, Location, Rotation, Scale, true, true, ENCPoolMethod::AutoRelease, true);

        if (NiagaraComponent)
        {
            FTimerHandle TimerHandle;
            MeshComp->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NiagaraComponent]()
                {
                    NiagaraComponent->DestroyComponent();
                }, 3.0f, false);
        }
    }

    // DoT
    AActor* OwningActor = MeshComp->GetOwner();
    AEnemyBase* Enemy = Cast<AEnemyBase>(OwningActor);
    if (!Enemy) return;

    FVector TargetLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
    float Distance = FVector::Dist(Enemy->GetActorLocation(), TargetLocation);

    if (Distance <= AttackTolerance)
    {
        TSubclassOf<UDamageType> DamageType;
        AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (!PlayerPawn) return;
        
        for (int32 i = 0; i <= TotalTicks; i++)
        {
            FTimerHandle DamageTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                DamageTimerHandle,
                [PlayerPawn, Enemy, DamageType]()
                {
                    float Damage = UGameplayStatics::ApplyDamage(
                        PlayerPawn,
                        Enemy->GetEnemyData()->Damage,
                        nullptr,
                        Enemy,
                        DamageType
                    );
                    UE_LOG(LogTemp, Warning, TEXT("%f damaged"), Damage);      
                },
                i * DamageInterval, false 
            );
        } 
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("It's too far to attack."));
    }

    UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
}
