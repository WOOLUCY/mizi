#pragma once
#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Containers/Array.h"
#include "Math/UnrealMathUtility.h"

class FUtils
{
public:

    template <typename T>
    static const T GetRandomElementFromArray(const TArray<T>& Array)
    {
        if (Array.Num() > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, Array.Num() - 1); 
            return Array[RandomIndex];
        }

        return T(); 
    }

    template<typename T>
    static const T RandomArrayItemFromStream(const TArray<T>& Array, FRandomStream& Stream)
    {
        // �迭�� ��� �ִ��� �˻�
        if (Array.Num() == 0)
        {
            // �迭�� ������� ���, �⺻�� ��ȯ (���� �ڵ鸵 ��Ŀ� ���� ���� ����)
            return T();
        }

        // ������ �ε����� �����Ͽ� �迭�� ��Ҹ� ��ȯ
        int32 RandomIndex = Stream.RandRange(0, Array.Num() - 1);
        return Array[RandomIndex];
    }
};
