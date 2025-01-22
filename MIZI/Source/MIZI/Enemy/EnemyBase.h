// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Interface/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "Data/EnemyData.h"
#include "NavigationInvokerComponent.h"

#include "EnemyBase.generated.h"

class ABasicAIController;
struct FEnemyTableRow;

UCLASS()
class MIZI_API AEnemyBase : public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	// Interface
	virtual float Attack() override;

public:
	FEnemyTableRow* GetEnemyData() const { return EnemyData; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	// Navigation Invoker Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
	UNavigationInvokerComponent* NavigationInvoker;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/MIZI.EnemyTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FEnemyTableRow* EnemyData;

	UAnimMontage* CurrentMontage;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP = 100.f;
};
