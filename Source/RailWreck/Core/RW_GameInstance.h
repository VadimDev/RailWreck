#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RailWreck/Scenario/RW_ScenarioData.h"
#include "RW_GameInstance.generated.h"

UCLASS()
class RAILWRECK_API URW_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	virtual void Init() override;

private:
	
	UPROPERTY() bool bIsScoringMode;
	
public:
	UFUNCTION(BlueprintCallable) bool GetScoringModeStatus() const { return bIsScoringMode; }
	UFUNCTION(BlueprintCallable) void SetScoringModeStatus(bool value) { bIsScoringMode = value; }
	
	void CreateEmptyScenario() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UScenarioData* ScenarioData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FScoringData ScoringData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EScoringMessageType ScenarioFinishingType;
};
