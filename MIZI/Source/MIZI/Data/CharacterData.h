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

	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TSubclassOf<UBasicCharacterAnimInstance> GunAnimClass;


public:	// Collision
	UPROPERTY(EditAnywhere, Category = "Character|Collision")
	float CollisionCapsuleRadius = 22.f;
	UPROPERTY(EditAnywhere, Category = "Character|Collision")
	float CollisionCapsuleHalfHeight = 44.f;

public: // Movement
	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float WalkMaxSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float SprintMaxSpeed = 800.f;
	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float StaminaRegenSpeed = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float StaminaDrainSpeed = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float WalkMinSpeed = 400.0f;
	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float WeightThreshold = 40.0f;
	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float WeightNoEffect = 10.0f;


public: // Animation
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TArray<UAnimMontage*> SignAttackMontage;
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TArray<UAnimMontage*> HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TArray<UAnimMontage*> DieMontage;

	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	UAnimMontage* RiffleEquipMontage;
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	UAnimMontage* RiffleUnEquipMontage;
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	UAnimMontage* RiffleFireMontage;
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	UAnimMontage* ReloadMontage;


public: // Scan
	UPROPERTY(EditAnywhere, Category = "Character|Scan")
	USoundBase* ScanSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Character|Scan")
	UCurveFloat* ScanTimeline = nullptr;

	UPROPERTY(EditAnywhere, Category = "Character|Scan")
	UMaterialParameterCollection* ScanCollection = nullptr;

	UPROPERTY(EditAnywhere, Category = "Character|Scan")
	float ScanRadiusMultiplier = 1500.0f;

public:	// UI
	UPROPERTY(EditAnywhere, Category = "Character|UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass = nullptr;



//public: // Weapon
	//UPROPERTY(EditAnywhere, Category = "Character|Weapon", meta = (RowType = "/Script/MIZI.WeaponTableRow"))
	//FDataTableRowHandle Weapon;
};