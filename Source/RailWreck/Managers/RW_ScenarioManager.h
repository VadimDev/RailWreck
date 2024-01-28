#pragma once

#include "CoreMinimal.h"
#include "EventSystem/BaseManager.h"
#include "RailWreck/GameObjects/RW_SpillActor.h"
#include "RW_ScenarioManager.generated.h"

class ATargetPoint;
class UScenarioData;
class URW_GameInstance;
class USpillEliminatedEvent;

UCLASS(Blueprintable)
class RAILWRECK_API URW_ScenarioManager : public UBaseManager
{
	GENERATED_BODY()

	DECLARE_EVENT_HANDLER(UInitializationEvent);
	DECLARE_EVENT_HANDLER(USpillEliminatedEvent);
	DECLARE_EVENT_HANDLER(UFinishScenarioEvent);
	DECLARE_EVENT_HANDLER(UFirstTimerExpiredEvent);
	DECLARE_EVENT_HANDLER(UFireExtinguishedEvent);
	DECLARE_EVENT_HANDLER(UTentConfirmEvent);
	DECLARE_EVENT_HANDLER(USetEquipmentEvent);
	DECLARE_EVENT_HANDLER(UOutletValveCloseEvent);
	DECLARE_EVENT_HANDLER(UUsedSlimeBombEvent);

public:
	URW_ScenarioManager();

	EEquipmentGear GetEquipmentGearByScenario(const EHazardousMaterial& InHazardousMaterial, bool InIsFire) const;
	void FinishScenario(EScoringMessageType FinishType);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	TSoftObjectPtr<AActor>FireTruck150Feet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	TSoftObjectPtr<AActor>FireTruck330Feet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	FName DayLightLevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	FName NightLightLevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	FName ChlorineCylindersLevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	FName ChlorineTankLevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	FName ChlorineContainerLevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	FName CrudeOilLevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	FName LiquidPropaneGasLevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	FName SulfuricAcidLevelName;

	UPROPERTY() URW_GameInstance* GameInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) UDataTable* DT_EquipmentGear;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	TMap<ESpillWidgetType, FText> SpillWidgetMessages;
	
private:
	void SetupSublevels(const UScenarioData* ScenarioData);
	void LoadSublevel(const FName& LevelName, const int32 UID) const;

	bool OutletValveClose = false;
	bool UsedSlimeBomb = false;
};
