#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RW_ScenarioSelectionWidget.generated.h"

class UCheckBox;
class UButtonsList;

UCLASS()
class RAILWRECK_API URW_ScenarioSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativePreConstruct() override;

	UFUNCTION()
	void OnButtonClicked(FText ButtonName);

public:
	UFUNCTION()
	void RandomizeScenario() const;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* CheckBox_Spill;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* CheckBox_Day;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* CheckBox_Fire;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButtonsList* ScenarioListButtons;
};
