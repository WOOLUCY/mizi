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
};
