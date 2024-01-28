#pragma once

#include "CoreMinimal.h"
#include "EventSystem/Events/BaseEvent.h"
#include "RailWreck/GameObjects/Equipment/RW_EquipmentActor.h"
#include "SetEquipmentEvent.generated.h"

UCLASS()
class RAILWRECK_API USetEquipmentEvent : public UBaseEvent
{
	GENERATED_BODY()

public:
	void Initialize(APlayerController* NewExecutor, const EEquipmentGear& NewEquipmentGear);
	
	EEquipmentGear GetEquipmentGear() const { return EquipmentGear; }

private:
	EEquipmentGear EquipmentGear;
};
