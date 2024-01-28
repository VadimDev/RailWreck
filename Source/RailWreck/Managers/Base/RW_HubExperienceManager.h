#pragma once

#include "CoreMinimal.h"
#include "EventSystem/ExperienceManager.h"
#include "RW_HubExperienceManager.generated.h"

class URW_HubButtonsManager;

UCLASS()
class RAILWRECK_API URW_HubExperienceManager : public UExperienceManager
{
	GENERATED_BODY()
	
public:
	//UFUNCTION(BlueprintCallable, Category="Managers") // todo
	//UTD_HubButtonsManager* GetHubButtonsManager() const;
};