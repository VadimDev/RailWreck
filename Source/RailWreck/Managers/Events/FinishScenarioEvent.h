#pragma once

#include "CoreMinimal.h"
#include "EventSystem/Events/BaseEvent.h"
#include "RailWreck/Scenario/RW_ScenarioData.h"
#include "UObject/Object.h"
#include "FinishScenarioEvent.generated.h"

UCLASS()
class RAILWRECK_API UFinishScenarioEvent : public UBaseEvent
{
	GENERATED_BODY()

public:
	void Initialize(APlayerController* NewExecutor, EScoringMessageType NewScoringFinishingType);
	EScoringMessageType GetScenarioFinishingType() const { return ScenarioFinishingType; }

private:
	UPROPERTY() EScoringMessageType ScenarioFinishingType;
};
