// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasicPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MIZI_API ABasicPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABasicPlayerState();

	uint32 GetCurHealth() const { return CurHealth; }
	uint32 GetCurStamina() const { return CurStamina; }
	uint32 GetCurSanity() const { return CurSanity; }
	uint32 GetMaxHealth() const { return MaxHealth; }
	uint32 GetMaxStamina() const { return MaxStamina; }
	uint32 GetMaxSanity() const { return MaxSanity; }

	void SetCurHealth(uint32 InUint) { CurHealth = InUint; }
	void SetCurStamina(uint32 InUint) { CurStamina = InUint; }
	void SetCurSanity(uint32 InUint) { CurSanity = InUint; }
	void SetMaxHealth(uint32 InUint) { MaxHealth = InUint; }
	void SetMaxStamina(uint32 InUint) { MaxStamina = InUint; }
	void SetMaxSanity(uint32 InUint) { MaxSanity = InUint; }

private:
	uint32 CurHealth = 100;
	uint32 MaxHealth = 100;

	uint32 CurStamina = 100;
	uint32 MaxStamina = 100;

	uint32 CurSanity = 100;
	uint32 MaxSanity = 100;
};
