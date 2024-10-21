#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "Engine/DataTable.h"

#include "ItemData.generated.h"

class UTexture2D;

USTRUCT()
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

public: // Base
	// 스태틱 메쉬 정보
	UPROPERTY(EditAnywhere, Category = "Item")
	UStaticMesh* StaticMesh = nullptr;

	// 장착 가능 여부
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bEquipped = true;


	// 사용 가능 여부
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bUsed = false;


	// 판매 가능 여부와 가격
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bSold = true;

	UPROPERTY(EditAnywhere, Category = "Item")
	uint32 MinPrice = 0;

	UPROPERTY(EditAnywhere, Category = "Item")
	uint32 MaxPrice = 100;

	UPROPERTY(EditAnywhere, Category = "Item")
	float Weight = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Item")
	FName EquipSocketName = TEXT("RightHand");

	UPROPERTY(EditAnywhere, Category = "Item")
	UTexture2D* Icon = nullptr;

public: // Animation
	//UPROPERTY(EditAnywhere, Category = "Item|Animation")
	//TSubclassOf<UBaisicCharacterAnimInstance> AnimClass;

	//UPROPERTY(EditAnywhere, Category = "Item|Animation")
	//UAnimMontage* WeaponAttackMontage;

};