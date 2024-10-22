#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "Animation/BasicCharacterAnimInstance.h"
#include "Engine/DataTable.h"

#include "CharacterData.generated.h"

USTRUCT()
struct MIZI_API FCharacterTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Character")
	FTransform MeshTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TSubclassOf<UBasicCharacterAnimInstance> AnimClass;

public:	// Collision
	UPROPERTY(EditAnywhere, Category = "Character|Collision")
	float CollisionCapsuleRadius = 22.f;
	UPROPERTY(EditAnywhere, Category = "Character|Collision")
	float CollisionCapsuleHalfHeight = 44.f;

public: // Movement
	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float MovementMaxSpeed = 400.f;

public: // Animation
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TArray<UAnimMontage*> HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TArray<UAnimMontage*> DieMontage;

public: // Scan
	UPROPERTY(EditAnywhere, Category = "Character|Scan")
	USoundBase* ScanSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Character|Scan")
	UCurveFloat* ScanTimeline = nullptr;

	UPROPERTY(EditAnywhere, Category = "Character|Scan")
	UMaterialParameterCollection* ScanCollection = nullptr;

	UPROPERTY(EditAnywhere, Category = "Character|Scan")
	float ScanRadiusMultiplier = 1500.0f;
//public: // Weapon
	//UPROPERTY(EditAnywhere, Category = "Character|Weapon", meta = (RowType = "/Script/MIZI.WeaponTableRow"))
	//FDataTableRowHandle Weapon;
};