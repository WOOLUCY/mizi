// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item/KeyCard.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/BasicCharacter.h"

void AKeyCard::OnUsed()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ABasicCharacter* BasicCharacter = Cast<ABasicCharacter>(Player);
	if (!BasicCharacter) return;
	if (!BasicCharacter->GetCanOpenDoor()) return;

	UE_LOG(LogTemp, Warning, TEXT("The KeyCard is Used"));
	BasicCharacter->GetOverlappedDoor()->OnOpen();

	// GetOwningItems()에서 TMap의 참조를 받아 수정
	auto& Items = BasicCharacter->GetOwningItems();
	uint32 Key = BasicCharacter->GetCurInventoryIndex();
	if (Items.Contains(Key))
	{
		Items.Remove(Key);
	}

	Destroy();
	Super::OnUsed();
}
