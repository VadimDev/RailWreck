#include "RW_ScoringWidgetActor.h"
#include "RW_ScoringWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RailWreck/Core/RW_GameInstance.h"
#include "RailWreck/Core/RW_HubGameMode.h"
#include "RailWreck/Managers/RW_UIManager.h"
#include "RailWreck/Managers/Base/RW_HubExperienceManager.h"

void ARW_ScoringWidgetActor::BeginPlay()
{
	Super::BeginPlay();

	const UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
	ScoringWidget = Cast<URW_ScoringWidget>(WidgetComponent->GetWidget());
	
	GameInstance = GetWorld()->GetGameInstance<URW_GameInstance>();
	if(!GameInstance->GetScoringModeStatus())
	{
		Destroy();
	}
	else
	{
		CalculateScoring();
	}
}

void ARW_ScoringWidgetActor::CalculateScoring() const
{
	if(!GameInstance->ScenarioData->IsFire)
	{
		GameInstance->ScoringData.bFireExtinguished = true;
	}
	else if(GameInstance->ScenarioData->IsFire)
	{
		GameInstance->ScoringData.bDecontaminationTentUsed = true;
	}

	if(!GameInstance->ScenarioData->IsSpill)
	{
		GameInstance->ScoringData.bSpillEliminated = true;
		GameInstance->ScoringData.bDecontaminationTentUsed = true;
	}

	if(GameInstance->ScoringData.bCorrectEquipment
		&& GameInstance->ScoringData.bFireExtinguished
		&& GameInstance->ScoringData.bSpillEliminated
		&& GameInstance->ScoringData.bDecontaminationTentUsed
		&& !GameInstance->ScoringData.bFirstTimerExpired
		&& GameInstance->ScoringData.bDetermineMaterialViaDetector
		&& GameInstance->ScoringData.bIsCorrectApproachDistance)
	{
		GameInstance->ScoringData.bGeneralSuccess = true;
	}

	UpdateScenarioMessage();
}

void ARW_ScoringWidgetActor::UpdateScenarioMessage() const
{
	if(ScoringMessagesAsset)
	{
		if(GameInstance->ScoringData.bGeneralSuccess)
		{
			const FText SuccessMessage = FText::FromString(*ScoringMessagesAsset->ResultMessages.Find(EScoringMessageType::SMT_Success));
			ScoringWidget->ResultMessage->SetText(SuccessMessage);
		}
		else if(const FString* FoundString = ScoringMessagesAsset->ResultMessages.Find(GameInstance->ScenarioFinishingType))
		{
			ScoringWidget->ResultMessage->SetText(FText::FromString(*FoundString));
		}
	}
}
