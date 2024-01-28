#pragma once

#include "CoreMinimal.h"
#include "EventSystem/ExperienceManager.h"
#include "GameFramework/GameMode.h"
#include "Interfaces/FlowCoreInterface.h"
#include "FlowVRGameMode.generated.h"

UCLASS()
class FLOWCORE_API AFlowVRGameMode : public AGameMode, public IFlowCoreInterface
{
	GENERATED_BODY()

public:

	AFlowVRGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	UExperienceManager* GetBaseExperienceManager() const { return ExperienceManager; }
	UFUNCTION()
	UBaseManager* GetManagerRef_Implementation(TSubclassOf<UBaseManager> ManagerClass);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Classes, DisplayName = ExperienceManager)
	TSubclassOf<UExperienceManager> ExperienceManagerClass;

	UPROPERTY()
	UExperienceManager* ExperienceManager = nullptr; 
};
