// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// UINTERFACE 매크로로 인터페이스 정의
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
    GENERATED_BODY()
};

// 실제 인터페이스 구현
class MIZI_API IEnemyInterface
{
    GENERATED_BODY()

public:
    // 인터페이스에 정의할 함수 선언 (순수 가상 함수)
    virtual float Attack() = 0;
};
