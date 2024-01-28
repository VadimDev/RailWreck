#include "RW_ApproachDistanceActor.h"
#include "Components/WidgetComponent.h"

URW_ApproachDistanceWidget* ARW_ApproachDistanceActor::GetApproachDistanceWidget() const
{
	const UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
	return Cast<URW_ApproachDistanceWidget>(WidgetComponent->GetWidget());
}
