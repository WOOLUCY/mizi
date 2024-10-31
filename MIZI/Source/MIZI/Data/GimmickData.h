#pragma once
#include "CoreMinimal.h"
#include "Misc/Utils.h"
#include "Engine/DataTable.h"

#include "GimmickData.generated.h"

//- Name
//- Serial Number
//- Static Mesh
//- Damage
//- Widget Class

USTRUCT()
struct MIZI_API FGimmickTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Gimmick")
	FText DisplayName;

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	int32 Damage = 20.f;

	// UI
	UPROPERTY(EditAnywhere, Category = "Gimmick|UI")
	TSubclassOf<UUserWidget> EnemyWidgetClass;
};