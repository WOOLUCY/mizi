#pragma once
#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Containers/Array.h"
#include "Math/UnrealMathUtility.h"

UENUM(BlueprintType)
enum class ETerminalCommand : uint8
{
    HELP = 0	UMETA(DisplayName = "Help"),
    MOONS		UMETA(DisplayName = "Moons"),
    STORE		UMETA(DisplayName = "Store"),
    BESTIARY	UMETA(DisplayName = "Bestiary"),

    ITEM  UMETA(DisplayName = "Item"),

    UNKNOWN		UMETA(DisplayName = "Unknown"),
};

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

    static const UInputAction* GetInputActionFromName(UInputMappingContext* IMC, const FName& InName)
    {
        const TArray<FEnhancedActionKeyMapping>& Mappings = IMC->GetMappings();
        for (auto& It : Mappings)
        {
            if (It.Action->GetFName() == InName)
            {
                return It.Action.Get();
            }
        }

        return nullptr;
    }
};
