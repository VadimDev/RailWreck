#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RW_ApproachDistanceWidget.generated.h"

class UScenarioData;
class UButtonWidget;
class UButtonsList;

UCLASS()
class RAILWRECK_API URW_ApproachDistanceWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativePreConstruct() override;

	UFUNCTION() void OnButtonClicked(FText ButtonName);
	UFUNCTION() void OnConfirmButtonClicked(FText ButtonName);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButtonsList* ApproachDistanceListButtons;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButtonWidget* ConfirmButton;
};
