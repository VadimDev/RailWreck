#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RailWreck/Scenario/RW_ScenarioData.h"
#include "RW_ScoringWidgetActor.generated.h"

class URW_ScoringWidget;
class URW_GameInstance;

UCLASS(BlueprintType)
class RAILWRECK_API UScoringMessagesAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EScoringMessageType, FString> ResultMessages;
};

UCLASS()
class RAILWRECK_API ARW_ScoringWidgetActor : public AActor
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	void CalculateScoring() const;
	void UpdateScenarioMessage() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup)
	UScoringMessagesAsset* ScoringMessagesAsset;
	
protected:
	UPROPERTY() URW_GameInstance* GameInstance;
	UPROPERTY() URW_ScoringWidget* ScoringWidget;
};
