#pragma once

#include "CoreMinimal.h"
#include "FlowVRGameMode.h"
#include "RW_HubGameMode.generated.h"

class UTD_HubExperienceManager;

UCLASS()
class RAILWRECK_API ARW_HubGameMode : public AFlowVRGameMode
{
	GENERATED_BODY()

public:
	ARW_HubGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	URW_HubExperienceManager* GetExperienceManager() const;
};
