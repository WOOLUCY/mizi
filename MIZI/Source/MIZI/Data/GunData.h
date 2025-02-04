#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/ItemData.h"

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
	// Base
	//UPROPERTY(EditAnywhere, Category = "Gun")
	//FText DisplayName;

	//UPROPERTY(EditAnywhere, Category = "Gun")
	//UStaticMesh* StaticMesh = nullptr;

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
	UPROPERTY(EditAnywhere, Category = "Gun|Effect")
	UParticleSystem* FireEffect;

	UPROPERTY(EditAnywhere, Category = "Gun|Effect")
	UParticleSystem* HitEffect;

	// Decal
	UPROPERTY(EditAnywhere, Category = "Gun|Decal")
	UMaterialInterface* BulletMarkDecal;


	// Value
	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	int32 CurBulletAmount;

	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	int32 MaxBulletAmount;

	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	int32 Damage;

	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	int32 Range;

	UPROPERTY(EditAnywhere, Category = "Gun|Value")
	int32 Price = 100;

	//UPROPERTY(EditAnywhere, Category = "Gun|Value")
	//float Weight = 10.0f;

	//UPROPERTY(EditAnywhere, Category = "Gun|Value")
	//FName EquipSocketName = TEXT("RightHand");
};