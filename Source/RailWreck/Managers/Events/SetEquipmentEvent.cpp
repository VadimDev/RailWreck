#include "SetEquipmentEvent.h"

void USetEquipmentEvent::Initialize(APlayerController* NewExecutor, const EEquipmentGear& NewEquipmentGear)
{
	Super::Initialize(NewExecutor);

	EquipmentGear = NewEquipmentGear;
}
