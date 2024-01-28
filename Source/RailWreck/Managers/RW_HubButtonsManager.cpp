#include "RW_HubButtonsManager.h"
#include "RW_UIManager.h"
#include "Base/RW_HubExperienceManager.h"
#include "EventSystem/Events/Experience/StartPlayEvent.h"
#include "Kismet/GameplayStatics.h"
#include "RailWreck/Core/RW_GameInstance.h"
#include "RailWreck/Core/RW_HubGameMode.h"
#include "RailWreck/GameObjects/RW_InteractionButton.h"
#include "RailWreck/UI/RW_ApproachDistanceActor.h"
#include "RailWreck/UI/RW_ScenarioSelectionWidget.h"
#include "RailWreck/UI/RW_ScenarioWidgetActor.h"
#include "RailWreck/UI/RW_ScoringWidgetActor.h"

URW_HubButtonsManager::URW_HubButtonsManager()
{
	DEFINE_EVENT_HANDLER(UStartPlayEvent);
}

void URW_HubButtonsManager::HandleEvent_Impl(UStartPlayEvent* Event)
{
	const ARW_HubGameMode* HubGameMode = Cast<ARW_HubGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
    ExperienceManager = HubGameMode->GetExperienceManager();
	UIManager = Cast<URW_UIManager>(ExperienceManager->GetManagerRef_Implementation(URW_UIManager::StaticClass()));

	ButtonsRefs.Find(EButtonType::EnterScenario)->Get()->SetButtonState(EButtonState::BS_Disabled);
}

void URW_HubButtonsManager::ButtonPress(ARW_InteractionButton* InteractionButton)
{
	URW_GameInstance* GI = Cast<URW_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	switch (InteractionButton->GetButtonType())
	{
	case EButtonType::SelectScenario:
		if(GI->GetScoringModeStatus())
		{
			UIManager->GetUIActorByType<ARW_ScoringWidgetActor>()->Destroy();
			GI->SetScoringModeStatus(false);
		}
		
		ButtonsRefs.Find(EButtonType::SelectScenario)->Get()->SetButtonState(EButtonState::BS_Disabled);
		ButtonsRefs.Find(EButtonType::RandomizeScenario)->Get()->SetButtonState(EButtonState::BS_Disabled);
		ButtonsRefs.Find(EButtonType::EnterScenario)->Get()->SetButtonState(EButtonState::BS_Default);
		
		SelectScenario(GI);
		break;
	case EButtonType::RandomizeScenario:
		RandomizeScenario();
		break;
	case EButtonType::ResetScenario:
		ButtonsRefs.Find(EButtonType::SelectScenario)->Get()->SetButtonState(EButtonState::BS_Default);
		ButtonsRefs.Find(EButtonType::RandomizeScenario)->Get()->SetButtonState(EButtonState::BS_Default);
		ButtonsRefs.Find(EButtonType::EnterScenario)->Get()->SetButtonState(EButtonState::BS_Disabled);
		ResetScenario(GI);
		break;
	case EButtonType::EnterScenario:
		ButtonsRefs.Find(EButtonType::SelectScenario)->Get()->SetButtonState(EButtonState::BS_Disabled);
		ButtonsRefs.Find(EButtonType::RandomizeScenario)->Get()->SetButtonState(EButtonState::BS_Disabled);
		ButtonsRefs.Find(EButtonType::ResetScenario)->Get()->SetButtonState(EButtonState::BS_Disabled);
		ButtonsRefs.Find(EButtonType::EnterScenario)->Get()->SetButtonState(EButtonState::BS_Disabled);
		EnterScenario();
		break;
	}
}

void URW_HubButtonsManager::RandomizeScenario() const
{
	UIManager->GetUIActorByType<ARW_ScenarioWidgetActor>()->GetScenarioSelectionWidget()->RandomizeScenario();
}

void URW_HubButtonsManager::SelectScenario(const URW_GameInstance* GI) const
{
	// hide scenario widget actor
	const auto ScenarioActor = Cast<ARW_ScenarioWidgetActor>(UIManager->GetUIActorByType<ARW_ScenarioWidgetActor>());
	ScenarioActor->SetActorHiddenInGame(true);
	ScenarioActor->SetActorEnableCollision(false);

	// set selected scenario
	UIManager->GetUIActorByType<ARW_ScenarioWidgetActor>()->GetSelectedParameters(GI);
}

void URW_HubButtonsManager::ResetScenario(const URW_GameInstance* GI) const
{
	UIManager->GetUIActorByType<ARW_ScenarioWidgetActor>()->ResetSelectedParameters(GI);
}

void URW_HubButtonsManager::EnterScenario() const
{
	UIManager->GetUIActorByType<ARW_ApproachDistanceActor>()->SetActorHiddenInGame(false);
}
