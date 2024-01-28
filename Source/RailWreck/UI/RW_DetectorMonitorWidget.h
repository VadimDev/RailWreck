#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RW_DetectorMonitorWidget.generated.h"

class UVerticalBox;
class USizeBox;
class UTextBlock;

UCLASS()
class RAILWRECK_API URW_DetectorMonitorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UTextBlock* MaterialType;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) USizeBox* ScanningInfo;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) UVerticalBox* FoundInfo;
};
