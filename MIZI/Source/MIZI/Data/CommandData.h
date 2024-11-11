#pragma once

#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "Engine/DataTable.h"

#include "CommandData.generated.h"

USTRUCT()
struct MIZI_API FCommandTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Command")
	ETerminalCommand Command;

	UPROPERTY(EditAnywhere, Category = "Command")
	FText OutputText;
};