#include "RW_ExpGameMode.h"

#include "RW_GameInstance.h"
#include "RailWreck/Managers/RW_ScenarioManager.h"
#include "RailWreck/Managers/Base/RW_ExperienceManager.h"

ARW_ExpGameMode::ARW_ExpGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	ExperienceManagerClass = UExperienceManager::StaticClass();
}

void ARW_ExpGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	const auto ScenarioData = GetWorld()->GetGameInstance<URW_GameInstance>()->ScenarioData;
	const auto ScenarioManager = Cast<URW_ScenarioManager>(ExperienceManager->GetManagerRef_Implementation(URW_ScenarioManager::StaticClass()));

	// debug
	//ScenarioData->HazardousMaterial = EHazardousMaterial::HM_Chlorine;
	//ScenarioData->ChlorineTrainType = EChlorineTrainType::CT_ChlorineCylinders;
	//ScenarioData->ApproachDistance = EApproachDistance::AD_150Feet;
	//ScenarioData->IsSpill = true;
	//ScenarioData->IsFire = true;
	//
	
	switch (ScenarioData->ApproachDistance)
	{
	case EApproachDistance::AD_75Feet:
	case EApproachDistance::AD_150Feet:
		if(ScenarioManager->FireTruck330Feet.Get())
		{
			ScenarioManager->FireTruck330Feet.Get()->Destroy();
			break;
		}
	case EApproachDistance::AD_330Feet:
		if(ScenarioManager->FireTruck150Feet.Get())
		{
			ScenarioManager->FireTruck150Feet.Get()->Destroy();
			break;
		}
	}

#if UE_EDITOR
	const FString MessageLog = "Scenario: Material - " + UEnum::GetDisplayValueAsText(
		ScenarioData->HazardousMaterial).ToString() +
	"; Approach Distance - " + UEnum::GetDisplayValueAsText(ScenarioData->ApproachDistance).ToString() +
	"; " + (ScenarioData->IsDay ? "Day" : "Night") +
	"; " + (ScenarioData->IsFire ? "Fire" : "No Fire") +
	"; " + (ScenarioData->IsSpill ? "Spill" : "No Spill") + ";";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *MessageLog);

	GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Green, MessageLog);
#endif

	FString ExperienceMapName;
	GConfig->GetString(TEXT("VRProjectSettings"), TEXT("ExperienceMapName"), ExperienceMapName, GGameIni);

	if (MapName.Contains(ExperienceMapName) || ExperienceMapName.IsEmpty())
	{
		ExperienceManager = NewObject<UExperienceManager>(this, ExperienceManagerClass);
		ExperienceManager->InitExperience();
	}
}

void ARW_ExpGameMode::StartPlay()
{
	Super::StartPlay();

	if (ExperienceManager)
	{
		ExperienceManager->StartExperience();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StartPlay__ExperienceManagerReference_Is_Not_Valid"));
	}
}

void ARW_ExpGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (ExperienceManager)
	{
		ExperienceManager->EndExperience();
	}
}

void ARW_ExpGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (ExperienceManager)
	{
		ExperienceManager->TickExperience(DeltaSeconds);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameModeTick__ExperienceManagerReference_Is_Not_Valid"));
	}
}

URW_ExperienceManager* ARW_ExpGameMode::GetExperienceManager() const
{
	return Cast<URW_ExperienceManager>(GetBaseExperienceManager());
}
