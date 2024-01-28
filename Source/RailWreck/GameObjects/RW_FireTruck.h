#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RW_FireTruck.generated.h"

UCLASS()
class RAILWRECK_API ARW_FireTruck : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnSpawnTentEvent();
};
