#pragma once

#include "CoreMinimal.h"
#include "FlowVRGameMode.h"
#include "RW_ExpGameMode.generated.h"

class URW_ExperienceManager;

UCLASS()
class RAILWRECK_API ARW_ExpGameMode : public AFlowVRGameMode
{
	GENERATED_BODY()

public:
	ARW_ExpGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	URW_ExperienceManager* GetExperienceManager() const;
};
