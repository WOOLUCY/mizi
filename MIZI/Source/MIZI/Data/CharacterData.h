#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
//#include "Animation/BaisicCharacterAnimInstance.h"

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

	//UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	//TSubclassOf<UBaisicCharacterAnimInstance> AnimClass;

public:	// Collision
	UPROPERTY(EditAnywhere, Category = "Character|Collision")
	float CollisionCapsuleRadius = 22.f;
	UPROPERTY(EditAnywhere, Category = "Character|Collision")
	float CollisionCapsuleHalfHeight = 44.f;

public: // Movement
	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float MovementMaxSpeed = 400.f;

public: // Animation
	//UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	//TArray<UAnimMontage*> HitReactMontage;
	//UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	//TArray<UAnimMontage*> DieMontage;

//public: // Weapon
	//UPROPERTY(EditAnywhere, Category = "Character|Weapon", meta = (RowType = "/Script/MIZI.WeaponTableRow"))
	//FDataTableRowHandle Weapon;
};