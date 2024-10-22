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
	UPROPERTY(EditAnywhere, Category = "Item")
	FText DisplayName;

	// ����ƽ �޽� ����
	UPROPERTY(EditAnywhere, Category = "Item")
	UStaticMesh* StaticMesh = nullptr;

	// ���� ���� ����
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bEquipped = true;


	// ��� ���� ����
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bUsed = false;


	// �Ǹ� ���� ���ο� ����
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