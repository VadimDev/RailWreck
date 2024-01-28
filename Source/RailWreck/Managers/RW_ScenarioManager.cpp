#include "RW_ScenarioManager.h"
#include "Events/FinishScenarioEvent.h"
#include "Events/FireExtinguishedEvent.h"
#include "Events/FirstTimerExpiredEvent.h"
#include "Events/OutletValveCloseEvent.h"
#include "Events/SetEquipmentEvent.h"
#include "Events/SpillEliminatedEvent.h"
#include "Events/TentConfirmEvent.h"
#include "Events/UsedSlimeBombEvent.h"
#include "EventSystem/Events/Experience/InitializationEvent.h"
#include "Kismet/GameplayStatics.h"
#include "RailWreck/Core/RW_GameInstance.h"
#include "RailWreck/GameObjects/RW_FireTruck.h"

URW_ScenarioManager::URW_ScenarioManager()
{
	DEFINE_EVENT_HANDLER(UInitializationEvent);
	DEFINE_EVENT_HANDLER(UFinishScenarioEvent);
	DEFINE_EVENT_HANDLER(UFirstTimerExpiredEvent);
	DEFINE_EVENT_HANDLER(UFireExtinguishedEvent);
	DEFINE_EVENT_HANDLER(USpillEliminatedEvent);
	DEFINE_EVENT_HANDLER(UTentConfirmEvent);
	DEFINE_EVENT_HANDLER(USetEquipmentEvent);
	DEFINE_EVENT_HANDLER(UOutletValveCloseEvent);
	DEFINE_EVENT_HANDLER(UUsedSlimeBombEvent);
}

void URW_ScenarioManager::HandleEvent_Impl(UInitializationEvent* Event)
{
	GameInstance = GetWorld()->GetGameInstance<URW_GameInstance>();
	const auto ScenarioData = GameInstance->ScenarioData;
	SetupSublevels(ScenarioData);
}

void URW_ScenarioManager::HandleEvent_Impl(UFirstTimerExpiredEvent* Event)
{
	UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::UFirstTimerExpiredEvent"));
	GameInstance->ScoringData.bFirstTimerExpired = true;
}

void URW_ScenarioManager::HandleEvent_Impl(UFireExtinguishedEvent* Event)
{
	UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::UFireExtinguishedEvent"));
	GameInstance->ScoringData.bFireExtinguished = true;
}

void URW_ScenarioManager::HandleEvent_Impl(USpillEliminatedEvent* Event)
{
	UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::USpillEliminatedEvent"));
	GameInstance->ScoringData.bSpillEliminated = true;
}

void URW_ScenarioManager::HandleEvent_Impl(UTentConfirmEvent* Event)
{
	UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::UTentConfirmEvent"));
	GameInstance->ScoringData.bDecontaminationTentUsed = true;

	if(FireTruck150Feet.IsValid())
	{
		Cast<ARW_FireTruck>(FireTruck150Feet.Get())->OnSpawnTentEvent();
	}
	else
	{
		Cast<ARW_FireTruck>(FireTruck330Feet.Get())->OnSpawnTentEvent();
	}
}

void URW_ScenarioManager::HandleEvent_Impl(UOutletValveCloseEvent* Event)
{
	UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::UOutletValveCloseEvent"));
	OutletValveClose = true;

	if(UsedSlimeBomb) SendEvent<USpillEliminatedEvent>(nullptr);
}

void URW_ScenarioManager::HandleEvent_Impl(UUsedSlimeBombEvent* Event)
{
	UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::UUsedSlimeBombEvent"));
	UsedSlimeBomb = true;

	if(OutletValveClose) SendEvent<USpillEliminatedEvent>(nullptr);
}

void URW_ScenarioManager::HandleEvent_Impl(UFinishScenarioEvent* Event)
{
	switch (Event->GetScenarioFinishingType())
	{
	case EScoringMessageType::SMT_Explosion:
		UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::SMT_Explosion"));
		FinishScenario(EScoringMessageType::SMT_Explosion);
		break;
	case EScoringMessageType::SMT_SecondFireTimerExpired: 
		UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::SMT_SecondFireTimerExpired"));
		FinishScenario(EScoringMessageType::SMT_SecondFireTimerExpired);
		break;
	case EScoringMessageType::SMT_SpillTimerExpired: 
		UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::SMT_SpillTimerExpired"));
		FinishScenario(EScoringMessageType::SMT_SpillTimerExpired);
		break;
	case EScoringMessageType::SMT_IncorrectEquipment: 
		UE_LOG(LogTemp, Warning, TEXT("URW_ScenarioManager::SMT_IncorrectEquipment"));
		FinishScenario(EScoringMessageType::SMT_IncorrectEquipment);
		break;
	}
}

void URW_ScenarioManager::HandleEvent_Impl(USetEquipmentEvent* Event)
{
	const auto ScenarioData = GetWorld()->GetGameInstance<URW_GameInstance>()->ScenarioData;
	const auto NeededEquipmentGear = GetEquipmentGearByScenario(ScenarioData->HazardousMaterial, ScenarioData->IsFire);
	
	if(NeededEquipmentGear == Event->GetEquipmentGear())
	{
		GameInstance->ScoringData.bCorrectEquipment = true;
	}
}

void URW_ScenarioManager::SetupSublevels(const UScenarioData* ScenarioData)
{
	ScenarioData->IsDay ? LoadSublevel(DayLightLevelName, 1) : LoadSublevel(NightLightLevelName, 1);
	
	switch (ScenarioData->HazardousMaterial)
	{
	case EHazardousMaterial::HM_None:
		UE_LOG(LogTemp, Warning, TEXT("EHazardousMaterial::HM_None -> ChlorineCylinders"));
		LoadSublevel(ChlorineCylindersLevelName, 2);
		break;
	case EHazardousMaterial::HM_CrudeOil:
		UE_LOG(LogTemp, Warning, TEXT("EHazardousMaterial::HM_CrudeOil -> CrudeOil"));
		LoadSublevel(CrudeOilLevelName, 2);
		break;
	case EHazardousMaterial::HM_Chlorine:
		switch(ScenarioData->ChlorineTrainType)
		{
		case EChlorineTrainType::CT_ChlorineCylinders:
			UE_LOG(LogTemp, Warning, TEXT("EHazardousMaterial::HM_Chlorine -> ChlorineCylinders"));
			LoadSublevel(ChlorineCylindersLevelName, 2);
			break;
		case EChlorineTrainType::CT_ChlorineTank:
			UE_LOG(LogTemp, Warning, TEXT("EHazardousMaterial::HM_Chlorine -> ChlorineTank"));
			LoadSublevel(ChlorineTankLevelName, 2);
			break;
		case EChlorineTrainType::CT_ChlorineContainer:
			UE_LOG(LogTemp, Warning, TEXT("EHazardousMaterial::HM_Chlorine -> ChlorineTon"));
			LoadSublevel(ChlorineContainerLevelName, 2);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("EHazardousMaterial::HM_Chlorine RandomInteger case default ->ChlorineCylinders"));
			LoadSublevel(ChlorineCylindersLevelName, 2);
		}
		break;
	case EHazardousMaterial::HM_LiquidPropaneGas:
		UE_LOG(LogTemp, Warning, TEXT("EHazardousMaterial::HM_LiquidPropaneGas -> LiquidPropaneGas"));
		LoadSublevel(LiquidPropaneGasLevelName, 2);
		break;
	case EHazardousMaterial::HM_SulfuricAcid:
		UE_LOG(LogTemp, Warning, TEXT("EHazardousMaterial::HM_SulfuricAcid -> SulfuricAcid"));
		LoadSublevel(SulfuricAcidLevelName, 2);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("HazardousMaterial case default -> ChlorineCylinders"));
		LoadSublevel(ChlorineCylindersLevelName, 2);
	}
}

void URW_ScenarioManager::LoadSublevel(const FName& LevelName, const int32 UID) const
{
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, true, {0, UID, {}, nullptr});
}

EEquipmentGear URW_ScenarioManager::GetEquipmentGearByScenario(const EHazardousMaterial& InHazardousMaterial,	const bool InIsFire) const
{
	EEquipmentGear Result;

	auto ForEach = [&Result, InHazardousMaterial, InIsFire](const FName& Key, const FEquipmentGearDataTable& Value)
	{
		if (Value.HazardousMaterial == InHazardousMaterial && Value.bIsFire == InIsFire)
		{
			Result = Value.EquipmentGear;
			return;
		}
	};
	
	DT_EquipmentGear->ForeachRow<FEquipmentGearDataTable>(
		TEXT("GetEquipmentGearByScenario(...) Expected RowStruct for this DT : FEquipmentGearDataTable"),
		ForEach);

	return Result;
}

void URW_ScenarioManager::FinishScenario(EScoringMessageType FinishType)
{
	GameInstance->SetScoringModeStatus(true);
	GameInstance->ScenarioFinishingType = FinishType;
	
	FString HubMapName;
	GConfig->GetString(TEXT("VRProjectSettings"), TEXT("HubMapName"), HubMapName, GGameIni);
	
	UGameplayStatics::OpenLevel(this, *HubMapName);
}