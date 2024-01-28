#include "RW_HubGameMode.h"

ARW_HubGameMode::ARW_HubGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	ExperienceManagerClass = UExperienceManager::StaticClass();
}

void ARW_HubGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString HubMapName;
	GConfig->GetString(TEXT("VRProjectSettings"), TEXT("HubMapName"), HubMapName, GGameIni);

	if (MapName.Contains(HubMapName) || HubMapName.IsEmpty())
	{
		ExperienceManager = NewObject<UExperienceManager>(this, ExperienceManagerClass);
		ExperienceManager->InitExperience();
	}
}

void ARW_HubGameMode::StartPlay()
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

void ARW_HubGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ExperienceManager)
	{
		ExperienceManager->EndExperience();
	}
}

void ARW_HubGameMode::Tick(float DeltaSeconds)
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

URW_HubExperienceManager* ARW_HubGameMode::GetExperienceManager() const
{
	return Cast<URW_HubExperienceManager>(GetBaseExperienceManager());
}
