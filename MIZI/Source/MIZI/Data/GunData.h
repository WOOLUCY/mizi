#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/ItemData.h"
#include "Actors/Weapon/BulletBase.h"

#include "GunData.generated.h"

//- Fire Sound
//- Equip sound
//- Unequip sound

//- Fire Camera Shake

//- Fire Effect
//- Hit Effect

//- Bullet Mark Decal

//- Cur Bullet Amount
//- Max Bullet Amount
//- Damage
//- Range


USTRUCT()
struct MIZI_API FGunTableRow : public FItemTableRow
{
	GENERATED_BODY()

public:
	// Bullet
	UPROPERTY(EditAnywhere, Category = "Gun|Bullet")
	TSubclassOf<ABulletBase> Bullet = nullptr;

	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.ProjectileTableRow"))
	FDataTableRowHandle ProjectileRowHandle;
	
	// Sound
	UPROPERTY(EditAnywhere, Category = "Gun|Sound")
	USoundBase* FireSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Gun|Sound")
	USoundBase* EquipSound = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Gun|Sound")
	USoundBase* UnequipSound = nullptr;

	// Camera Shake
	UPROPERTY(EditAnywhere, Category = "Gun|CS")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	// Effect
	//UPROPERTY(EditAnywhere, Category = "Gun|Effect")
	//UParticleSystem* FireEffect;

	// Value
	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	int32 CurBulletAmount;

	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	int32 MaxBulletAmount;


	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	int32 Damage;			// 피해량


	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	int32 Range;			// 사거리

	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	float FireSpeed = 0.1f;	// 연사 속도
};