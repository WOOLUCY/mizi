#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "Engine/DataTable.h"
#include "Animation/EnemyAnimInstance.h"
//#include "BehaviorTree/BehaviorTree.h"
//#include "Enemy/BasicAIController.h"

#include "EnemyData.generated.h"

class UBehaviorTree;
class UEnemyAnimInstance;
class UEnemyAnimInstance;

USTRUCT()
struct MIZI_API FEnemyTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Enemy")
	FText DisplayName;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	FTransform MeshTransform = FTransform::Identity;

public:	// Collision
	UPROPERTY(EditAnywhere, Category = "Enemy|Collision")
	float CollisionCapsuleRadius = 22.f;
	UPROPERTY(EditAnywhere, Category = "Enemy|Collision")
	float CollisionCapsuleHalfHeight = 44.f;

public: // Movement
	UPROPERTY(EditAnywhere, Category = "Enemy|Movement")
	float WalkMaxSpeed = 600.f;

public: // Animation
	UPROPERTY(EditAnywhere, Category = "Enemy|Animation")
	TSubclassOf<UEnemyAnimInstance> AnimClass;
	UPROPERTY(EditAnywhere, Category = "Enemy|Animation")
	TArray<UAnimMontage*> HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Enemy|Animation")
	TArray<UAnimMontage*> AttackMontage;
	UPROPERTY(EditAnywhere, Category = "Enemy|Animation")
	TArray<UAnimMontage*> DieMontage;

public: // Combat
	UPROPERTY(EditAnywhere, Category = "Enemy|Combat")
	int32 Damage;

public:	// UI
	UPROPERTY(EditAnywhere, Category = "Enemy|UI")
	TSubclassOf<UUserWidget> EnemyWidgetClass;

	//public: // Weapon
		//UPROPERTY(EditAnywhere, Category = "Character|Weapon", meta = (RowType = "/Script/MIZI.WeaponTableRow"))
		//FDataTableRowHandle Weapon;
};