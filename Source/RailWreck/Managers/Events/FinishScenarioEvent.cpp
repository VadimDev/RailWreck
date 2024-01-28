#include "FinishScenarioEvent.h"
#include "RailWreck/Scenario/RW_ScenarioData.h"

void UFinishScenarioEvent::Initialize(APlayerController* NewExecutor, EScoringMessageType NewScenarioFinishingType)
{
	Super::Initialize(NewExecutor);

	ScenarioFinishingType = NewScenarioFinishingType;
}
