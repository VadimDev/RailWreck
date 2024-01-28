#include "RW_ApproachDistanceWidget.h"

#include "Kismet/GameplayStatics.h"
#include "RailWreck/Core/RW_GameInstance.h"
#include "UI/ButtonsList.h"
#include "UI/ButtonWidget.h"

void URW_ApproachDistanceWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	ApproachDistanceListButtons->OnButtonClickEventDelegate.AddUniqueDynamic(this, &URW_ApproachDistanceWidget::OnButtonClicked);
	ConfirmButton->OnClickEventDelegate.AddUniqueDynamic(this, &URW_ApproachDistanceWidget::OnConfirmButtonClicked);
}

void URW_ApproachDistanceWidget::OnButtonClicked(FText ButtonName)
{
	const auto ScenarioData = Cast<URW_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ScenarioData;
	
	if(ButtonName.EqualTo(FText::FromString("75 feet")))
	{
		ScenarioData->ApproachDistance = EApproachDistance::AD_75Feet;
	}
	else if(ButtonName.EqualTo(FText::FromString("150 feet")))
	{
		ScenarioData->ApproachDistance = EApproachDistance::AD_150Feet;
	}
	else if(ButtonName.EqualTo(FText::FromString("330 feet")))
	{
		ScenarioData->ApproachDistance = EApproachDistance::AD_330Feet;
	}
}

void URW_ApproachDistanceWidget::OnConfirmButtonClicked(FText ButtonName)
{
	const auto GameInstance = Cast<URW_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	const auto ScenarioData = GameInstance->ScenarioData;
	
	if(ScenarioData->IsFire || ScenarioData->IsSpill)
	{
		if(ScenarioData->ApproachDistance != EApproachDistance::AD_330Feet)
		{
			GameInstance->ScoringData.bIsCorrectApproachDistance = false;
		}
		else
		{
			GameInstance->ScoringData.bIsCorrectApproachDistance = true;
		}
	}
	else
	{
		if(ScenarioData->ApproachDistance == EApproachDistance::AD_75Feet
			|| ScenarioData->ApproachDistance == EApproachDistance::AD_330Feet)
		{
			GameInstance->ScoringData.bIsCorrectApproachDistance = false;
		}
		else
		{
			GameInstance->ScoringData.bIsCorrectApproachDistance = true;
		}
	}
	
	FString ExperienceMapName;
	GConfig->GetString(TEXT("VRProjectSettings"), TEXT("ExperienceMapName"), ExperienceMapName, GGameIni);
	
	UGameplayStatics::OpenLevel(this, *ExperienceMapName);
}
