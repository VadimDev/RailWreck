#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RW_ScenarioWidgetActor.generated.h"

class URW_GameInstance;
class UScenarioData;
class URW_ScenarioSelectionWidget;

UCLASS()
class RAILWRECK_API ARW_ScenarioWidgetActor : public AActor
{
	GENERATED_BODY()

public:
	URW_ScenarioSelectionWidget* GetScenarioSelectionWidget() const;
	void GetSelectedParameters(const URW_GameInstance* GI) const;
	void ResetSelectedParameters(const URW_GameInstance* GI);
};