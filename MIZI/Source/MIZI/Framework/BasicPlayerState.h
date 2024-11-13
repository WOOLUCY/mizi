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

	int32 GetCurHealth() const { return CurHealth; }
	int32 GetCurStamina() const { return CurStamina; }
	int32 GetCurSanity() const { return CurSanity; }
	int32 GetMaxHealth() const { return MaxHealth; }
	int32 GetMaxStamina() const { return MaxStamina; }
	int32 GetMaxSanity() const { return MaxSanity; }
	int32 GetCurMoney() const { return CurMoney; }

	void SetCurHealth(int32 InUint) { CurHealth = InUint; }
	void SetCurStamina(int32 InUint) { CurStamina = InUint; }
	void SetCurSanity(int32 InUint) { CurSanity = InUint; }
	void SetMaxHealth(int32 InUint) { MaxHealth = InUint; }
	void SetMaxStamina(int32 InUint) { MaxStamina = InUint; }
	void SetMaxSanity(int32 InUint) { MaxSanity = InUint; }
	void SetCurMoney(int32 InUint) { CurMoney = InUint; }

public:
	UPROPERTY(BlueprintReadWrite)
	int32 CurHealth;
	UPROPERTY(BlueprintReadWrite)
	int32 MaxHealth;

	UPROPERTY(BlueprintReadWrite)
	int32 CurStamina;
	UPROPERTY(BlueprintReadWrite)
	int32 MaxStamina;
			
	UPROPERTY(BlueprintReadWrite)
	int32 CurSanity;
	UPROPERTY(BlueprintReadWrite)
	int32 MaxSanity;

	UPROPERTY(BlueprintReadWrite)
	int32 CurMoney;
};
