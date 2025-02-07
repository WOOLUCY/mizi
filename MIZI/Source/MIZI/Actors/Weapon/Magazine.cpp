// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/Magazine.h"
#include "PlayerCharacter/BasicCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AMagazine::AMagazine()
{
	ItemName = TEXT("Magazine");
}

void AMagazine::OnUsed()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	// 장전 소리 
	if (ReloadSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, GetActorLocation());
	}

	// 장전 애니메이션 몽타주 재생
	float Duration = Character->PlayAnimMontage(Character->GetCharacterData()->ReloadMontage);
	Duration -= 1;

	// 라이플을 찾아와서 찾는 동안만 손에 보이기
	for (auto& Pair : Character->GetOwningItems())
	{
		if (Pair.Value->GetClass() != GunClass)	continue;

		Pair.Value->SetActorHiddenInGame(false);
		break;
	}

	// 딜레이 후, 총알 개수 늘림
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("ReloadBullets"), Duration, false);
}

void AMagazine::ReloadBullets()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	for (auto& Pair : Character->GetOwningItems())
	{
		AGunBase* Gun = Cast<AGunBase>(Pair.Value);

		if (Gun)
		{
			// 총알 개수를 늘림
			Gun->SetCurBulletAmount(Gun->GetMaxBulletAmount());

			// 사용된 총알집은 삭제
			auto& Items = Character->GetOwningItems();
			uint32 Key = Character->GetCurInventoryIndex();
			if (Items.Contains(Key))
			{
				Items.Remove(Key);
			}

			Super::OnUsed();
			Destroy();

			break;
		}
	}
}
