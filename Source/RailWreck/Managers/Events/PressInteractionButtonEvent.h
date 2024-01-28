#pragma once

#include "CoreMinimal.h"
#include "EventSystem/Events/BaseEvent.h"
#include "UObject/Object.h"
#include "PressInteractionButtonEvent.generated.h"

enum class EButtonType : uint8;

UCLASS()
class RAILWRECK_API UPressInteractionButtonEvent : public UBaseEvent
{
	GENERATED_BODY()

public:
	void Initialize(APlayerController* NewExecutor, EButtonType ButtonType);
};
