// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Maynard.h"
#include "AIController.h"         
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Utils.h"


float AMaynard::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GetMovementComponent()->StopMovementImmediately();

	float NewHP = HP - DamageAmount;
	if (NewHP <= 0)
	{
		TArray<UAnimMontage*> DieMontages = EnemyData->DieMontage;
		float AnimationDuration = PlayAnimMontage(FUtils::GetRandomElementFromArray(DieMontages));
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnDie"), AnimationDuration + 0.5f, false);
		return 0.0f;
	}


	// 블랙보드에서 피격 당하고 있음을 알리기
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(TEXT("IsAttacked"), true);
		}
	}

	TArray<UAnimMontage*> ReactMontages = EnemyData->HitReactMontage;
	if (ReactMontages.IsEmpty())
	{
		ensure(false);
		return 0.f;
	}
	float AnimationDuration = PlayAnimMontage(ReactMontages[0]);
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("SetCanMove"), AnimationDuration + 0.5f, false);

	HP = NewHP;

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMaynard::SetCanMove()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(TEXT("IsAttacked"), false);
		}
	}
}

void AMaynard::OnDie()
{
	Destroy();
}
