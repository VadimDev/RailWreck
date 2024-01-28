#pragma once

#include "CoreMinimal.h"
#include "EventSystem/BaseManager.h"
#include "RailWreck/GameObjects/RW_FireActor.h"
#include "UObject/Object.h"
#include "RW_FireManager.generated.h"

UINTERFACE(MinimalAPI)
class URW_FirefightingInterface : public UInterface
{
	GENERATED_BODY()
};

class RAILWRECK_API IRW_FirefightingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firefighting")
	void GroundMonitorSpraying();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firefighting")
	void FoamNozzleSpraying();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Firefighting")
	void WaterNozzleSpraying();
};

UCLASS(Blueprintable)
class RAILWRECK_API URW_FireManager : public UBaseManager
{
	GENERATED_BODY()

public:
	URW_FireManager();
	
	DECLARE_EVENT_HANDLER(USecondTimerExpiredEvent);

	UFUNCTION(BlueprintCallable) void FireIsSpawned(ARW_FireActor * FireActor);
	
	FFireDataTable GetFireSettingsByScenario(const EHazardousMaterial& InHazardousMaterial) const;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) UDataTable* DT_Fire;
	UPROPERTY(BlueprintReadWrite) TArray<ARW_FireActor*> FireActors;
	
private:
	UFUNCTION() void FireExtinguished(ARW_FireActor* InFireActor);
	UFUNCTION() void UncontrollableFire(ARW_FireActor* InFireActor);
};
