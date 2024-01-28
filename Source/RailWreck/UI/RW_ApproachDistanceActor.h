#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RW_ApproachDistanceActor.generated.h"

class URW_ApproachDistanceWidget;

UCLASS()
class RAILWRECK_API ARW_ApproachDistanceActor : public AActor
{
	GENERATED_BODY()

public:
	URW_ApproachDistanceWidget* GetApproachDistanceWidget() const;
};