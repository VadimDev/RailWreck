#include "RW_ScenarioWidgetActor.h"
#include "RW_ScenarioSelectionWidget.h"
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetComponent.h"
#include "RailWreck/Core/RW_GameInstance.h"
#include "RailWreck/Scenario/RW_ScenarioData.h"
#include "UI/ButtonsList.h"
#include "UI/ButtonWidget.h"

URW_ScenarioSelectionWidget* ARW_ScenarioWidgetActor::GetScenarioSelectionWidget() const
{
	const UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
	return Cast<URW_ScenarioSelectionWidget>(WidgetComponent->GetWidget());
}

void ARW_ScenarioWidgetActor::GetSelectedParameters(const URW_GameInstance* GI) const
{
	const auto ScenarioData = GI->ScenarioData;
	const auto ScenarioSelectionWidget = GetScenarioSelectionWidget();
	const auto ButtonsContainer = ScenarioSelectionWidget->ScenarioListButtons->ButtonsContainer;
	
	for(uint8 Index = 0; Index < ButtonsContainer->GetChildrenCount(); Index++)
	{
		if(const auto Button = Cast<UButtonWidget>(ButtonsContainer->GetChildAt(Index)); Button->GetState())
		{
			switch (Index + 1) // Due to the fact that the enumeration start from element HM_None = 0
			{
			case static_cast<uint8>(EHazardousMaterial::HM_CrudeOil):
				ScenarioData->HazardousMaterial = EHazardousMaterial::HM_CrudeOil;
				break;
			case static_cast<uint8>(EHazardousMaterial::HM_Chlorine):
				ScenarioData->HazardousMaterial = EHazardousMaterial::HM_Chlorine;
				// choose one of three random train types
				ScenarioData->ChlorineTrainType = static_cast<EChlorineTrainType>(FMath::RandRange(0, 2));
				break;
			case static_cast<uint8>(EHazardousMaterial::HM_LiquidPropaneGas):
				ScenarioData->HazardousMaterial = EHazardousMaterial::HM_LiquidPropaneGas;
				break;
			case static_cast<uint8>(EHazardousMaterial::HM_SulfuricAcid):
				ScenarioData->HazardousMaterial = EHazardousMaterial::HM_SulfuricAcid;
				break;
			}
		}
	}
	
	ScenarioData->IsSpill = ScenarioSelectionWidget->CheckBox_Spill->IsChecked();
	ScenarioData->IsDay = ScenarioSelectionWidget->CheckBox_Day->IsChecked();
	ScenarioData->IsFire = ScenarioSelectionWidget->CheckBox_Fire->IsChecked();

	UE_LOG(LogTemp, Warning,
		TEXT("HazardousMaterial = %s, IsSpill = %i, IsDay = %i, IsFire = %i"),
			*UEnum::GetDisplayValueAsText(ScenarioData->HazardousMaterial).ToString(),
			ScenarioData->IsSpill,
			ScenarioData->IsDay,
			ScenarioData->IsFire);
}

void ARW_ScenarioWidgetActor::ResetSelectedParameters(const URW_GameInstance* GI)
{
	GetScenarioSelectionWidget()->CheckBox_Spill->SetCheckedState(ECheckBoxState::Unchecked);
	GetScenarioSelectionWidget()->CheckBox_Day->SetCheckedState(ECheckBoxState::Checked);
	GetScenarioSelectionWidget()->CheckBox_Fire->SetCheckedState(ECheckBoxState::Unchecked);
	GetScenarioSelectionWidget()->CheckBox_Fire->SetIsEnabled(true);

	const auto ScenarioListButtons = GetScenarioSelectionWidget()->ScenarioListButtons;
	
	for (const auto w : ScenarioListButtons->ButtonsContainer->GetAllChildren())
	{
		Cast<UButtonWidget>(w)->ChangeButtonState(false);
	}
	
	Cast<UButtonWidget>(ScenarioListButtons->ButtonsContainer->GetChildAt(0))->ChangeButtonState(true);

	GI->CreateEmptyScenario();
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}