#include "RW_ScenarioSelectionWidget.h"
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"
#include "UI/ButtonsList.h"
#include "UI/ButtonWidget.h"

void URW_ScenarioSelectionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	ScenarioListButtons->OnButtonClickEventDelegate.AddUniqueDynamic(this, &URW_ScenarioSelectionWidget::OnButtonClicked);
}

void URW_ScenarioSelectionWidget::OnButtonClicked(FText ButtonName)
{
	if(ButtonName.EqualTo(FText::FromString("Chlorine")))
	{
		CheckBox_Fire->SetCheckedState(ECheckBoxState::Unchecked);
		CheckBox_Fire->SetIsEnabled(false);
	}
	else
	{
		CheckBox_Fire->SetIsEnabled(true);
	}
}

void URW_ScenarioSelectionWidget::RandomizeScenario() const
{
	for (const auto w : ScenarioListButtons->ButtonsContainer->GetAllChildren())
	{
		Cast<UButtonWidget>(w)->ChangeButtonState(false);
		CheckBox_Fire->SetIsEnabled(true);
	}
	
	const int32 RandomInteger = FMath::RandRange(0, ScenarioListButtons->ButtonsContainer->GetChildrenCount()-1);

	const auto ButtonWidget = Cast<UButtonWidget>(ScenarioListButtons->ButtonsContainer->GetChildAt(RandomInteger));
	ButtonWidget->ChangeButtonState(true);
	
	TArray<UCheckBox*> CheckBoxArray;
	
	CheckBoxArray.Add(CheckBox_Spill);
	CheckBoxArray.Add(CheckBox_Day);
	CheckBoxArray.Add(CheckBox_Fire);

	for(const auto CheckBox : CheckBoxArray)
	{
		if(ButtonWidget->GetButtonName().EqualTo(FText::FromString("Chlorine")))
		{
			CheckBox_Fire->SetCheckedState(ECheckBoxState::Unchecked);
			CheckBox_Fire->SetIsEnabled(false);
		}
		
		if(FMath::RandBool() == true)
		{
			CheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else
		{
			CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		}
	}
}