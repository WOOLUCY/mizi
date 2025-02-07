// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item/RifleBullet.h"

#include "Actors/Weapon/Rifle.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter/BasicCharacter.h"

ARifleBullet::ARifleBullet()
{
	ItemName = TEXT("RifleBullet");
}

void ARifleBullet::OnUsed()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	// 장전 애니메이션 몽타주 재생
	float Duration = Character->PlayAnimMontage(Character->GetCharacterData()->ReloadMontage);
	Duration -= 1;

	// 라이플을 찾아와서 찾는 동안만 손에 보이기
	for (auto& Pair : Character->GetOwningItems())
	{
		AItemBase* Item = Pair.Value;
		ARifle* Rifle = Cast<ARifle>(Item);

		if (Rifle)
		{
			Rifle->SetActorHiddenInGame(false);
			break;
		}
	}

	// 딜레이 후, 총알 개수 늘림
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("ReloadBullets"), Duration, false);

}

void ARifleBullet::ReloadBullets()
{
	ABasicCharacter* Character = Cast<ABasicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Character)
	{
		ensure(false);
		return;
	}

	for (auto& Pair : Character->GetOwningItems())
	{
		AItemBase* Item = Pair.Value;
		ARifle* Rifle = Cast<ARifle>(Item);

		if (Rifle)
		{
			// 라이플의 총알 개수를 늘림
			int32 NewAmount = Rifle->GetCurBulletAmount() + BulletAmount;
			NewAmount = UKismetMathLibrary::Clamp(NewAmount, 0, Rifle->GetMaxBulletAmount());
			Rifle->SetCurBulletAmount(NewAmount);

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
