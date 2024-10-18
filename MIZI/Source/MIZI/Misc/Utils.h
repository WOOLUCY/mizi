#pragma once
#include "CoreMinimal.h"

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
        // 배열이 비어 있는지 검사
        if (Array.Num() == 0)
        {
            // 배열이 비어있을 경우, 기본값 반환 (에러 핸들링 방식에 따라 수정 가능)
            return T();
        }

        // 랜덤한 인덱스를 생성하여 배열의 요소를 반환
        int32 RandomIndex = Stream.RandRange(0, Array.Num() - 1);
        return Array[RandomIndex];
    }
};
