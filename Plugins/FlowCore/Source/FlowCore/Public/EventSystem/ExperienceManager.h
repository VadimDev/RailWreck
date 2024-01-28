#pragma once

#include "CoreMinimal.h"
#include "Interfaces/FlowCoreInterface.h"
#include "UObject/Object.h"
#include "ExperienceManager.generated.h"

class AController;
class UBaseManager;
class UBaseEvent;

UCLASS(Blueprintable)
class FLOWCORE_API UExperienceManager : public UObject, public IFlowCoreInterface
{
	GENERATED_BODY()

public:
	virtual void InitExperience();
	virtual void StartExperience();
	virtual void TickExperience(float DeltaTime);
	virtual void EndExperience();
	virtual void PrepareScenario(APlayerController* NewPlayer){};

	UFUNCTION()
	UBaseManager* GetManagerRef_Implementation(TSubclassOf<UBaseManager> ManagerClass);

private:
	void HandleEvents() const;
	void DispatchEvent(UBaseEvent* Event) const;

protected:
	/** Classes for different event-based managers, used in game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Classes, DisplayName = SingleInstanceListeners)
	TArray<TSubclassOf<UBaseManager>> SingleListenerClasses = { };
	
	UPROPERTY()	TArray<UBaseManager*> SingleListeners;

	FRWLock Lock;
};