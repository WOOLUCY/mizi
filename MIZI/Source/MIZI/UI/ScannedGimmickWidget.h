#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"

#include "ScannedEnemyWidget.h"

#include "ScannedGimmickWidget.generated.h"

UCLASS()
class MIZI_API UScannedGimmickWidget : public UScannedEnemyWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SerialNumber;
};
