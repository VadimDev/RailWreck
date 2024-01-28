#include "RW_WristMenuManager.h"

#include "RW_ScenarioManager.h"
#include "Base/RW_ExperienceManager.h"
#include "Base/RW_HubExperienceManager.h"
#include "EventSystem/Events/Experience/InitializationEvent.h"
#include "Kismet/GameplayStatics.h"
#include "RailWreck/Core/RW_ExpGameMode.h"
#include "RailWreck/Core/RW_PlayerCharacter.h"
#include "RailWreck/GameObjects/Equipment/RW_Detector.h"

URW_WristMenuManager::URW_WristMenuManager()
{
	DEFINE_EVENT_HANDLER(UInitializationEvent);
}

void URW_WristMenuManager::HandleEvent_Impl(UInitializationEvent* Event)
{
	if(const auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		Cast<ARW_PlayerCharacter>(PlayerCharacter)->WristMenuDispatcher.AddUniqueDynamic(this, &URW_WristMenuManager::OnWristMenuExecuted);
	}
}

void URW_WristMenuManager::OnWristMenuExecuted(FString Option, ARW_PlayerCharacter* PlayerCharacter)
{
	if (Option.Equals("ExitGame"))
	{
		const auto ExpGameMode = Cast<ARW_ExpGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		const auto ExperienceManager = ExpGameMode->GetExperienceManager();
		const auto ScenarioManager = Cast<URW_ScenarioManager>(ExperienceManager->GetManagerRef_Implementation(URW_ScenarioManager::StaticClass()));

		ScenarioManager->FinishScenario(EScoringMessageType::SMT_LeftTheTraining);
	}
	else if (Option.Equals("Detector"))
	{
		if(MaterialDetectorClass)
		{
			const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
		
			const auto MaterialDetector = GetWorld()->SpawnActor<ARW_Detector>(MaterialDetectorClass, FVector(), FRotator());
			MaterialDetector->AttachToComponent(PlayerCharacter->VRHandLeft->HandMesh, AttachmentTransformRules);
			PlayerCharacter->VRHandLeft->GrabObject();
		}
	}
}