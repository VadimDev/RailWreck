#pragma once

#include "CoreMinimal.h"
#include "EventSystem/BaseManager.h"
#include "Events/PressInteractionButtonEvent.h"
#include "RW_HubButtonsManager.generated.h"

class URW_GameInstance;
class URW_UIManager;
class URW_HubExperienceManager;
class ARW_InteractionButton;

enum class EButtonType : uint8;

UCLASS(Blueprintable)
class RAILWRECK_API URW_HubButtonsManager : public UBaseManager
{
	GENERATED_BODY()
	
	DECLARE_EVENT_HANDLER(UStartPlayEvent)
	
	URW_HubButtonsManager();
	
	void RandomizeScenario() const;
	void SelectScenario(const URW_GameInstance* GI) const;
	void ResetScenario(const URW_GameInstance* GI) const;
	void EnterScenario() const;

	UPROPERTY() URW_HubExperienceManager* ExperienceManager = nullptr;
	UPROPERTY() URW_UIManager* UIManager = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setups")
	TMap<EButtonType, TSoftObjectPtr<ARW_InteractionButton>> ButtonsRefs;
	
	UFUNCTION()
	void ButtonPress(ARW_InteractionButton* InteractionButton);
};