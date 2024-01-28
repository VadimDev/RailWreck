#pragma once

#include "CoreMinimal.h"
#include "EventSystem/BaseManager.h"
#include "UObject/Object.h"
#include "RW_WristMenuManager.generated.h"

class ARW_PlayerCharacter;

UCLASS(Blueprintable)
class RAILWRECK_API URW_WristMenuManager : public UBaseManager
{
	GENERATED_BODY()

	DECLARE_EVENT_HANDLER(UInitializationEvent);

public:
	URW_WristMenuManager();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AActor> MaterialDetectorClass;

private:
	UFUNCTION()
	virtual void OnWristMenuExecuted(FString Option, ARW_PlayerCharacter* PlayerCharacter);
};
