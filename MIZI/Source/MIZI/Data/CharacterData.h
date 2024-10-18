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
	UPROPERTY(EditAnywhere, Category = "Pawn")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	FTransform MeshTransform = FTransform::Identity;

	//UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	//TSubclassOf<UBaisicCharacterAnimInstance> AnimClass;

public: // Animation
	//UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	//TArray<UAnimMontage*> HitReactMontage;
	//UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	//TArray<UAnimMontage*> DieMontage;

//public: // Weapon
//	UPROPERTY(EditAnywhere, Category = "Pawn|Weapon", meta = (RowType = "/Script/KDT3D.WeaponTableRow"))
//	FDataTableRowHandle Weapon;
};