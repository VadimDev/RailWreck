#include "RW_FireManager.h"
#include "Events/FinishScenarioEvent.h"
#include "Events/FireExtinguishedEvent.h"
#include "Events/SecondTimerExpiredEvent.h"
#include "RailWreck/Core/RW_GameInstance.h"

URW_FireManager::URW_FireManager()
{
	DEFINE_EVENT_HANDLER(USecondTimerExpiredEvent);
}

void URW_FireManager::FireIsSpawned(ARW_FireActor * FireActor)
{
	URW_GameInstance* GameInstance = GetWorld()->GetGameInstance<URW_GameInstance>();
	const auto ScenarioData = GameInstance->ScenarioData;

	if(GameInstance->ScenarioData->IsFire)
	{
		if(FireActor)
		{
			FireActor->OnFireExtinguished().AddUniqueDynamic(this, &URW_FireManager::FireExtinguished);
			FireActor->OnUncontrollableFire().AddDynamic(this, &URW_FireManager::UncontrollableFire);
			const FFireDataTable FireSettings = GetFireSettingsByScenario(ScenarioData->HazardousMaterial);
			FireActor->SetFirefightingTools(FireSettings.FirefightingTools);
			FireActor->SetFireShrinkTime(FireSettings.FireShrinkTime);
			FireActor->SetProhibitedFirefightingTools(FireSettings.ProhibitedFirefightingTools);
		}
	}
}

FFireDataTable URW_FireManager::GetFireSettingsByScenario(const EHazardousMaterial& InHazardousMaterial) const
{
	FFireDataTable Result;

	auto ForEach = [&Result, InHazardousMaterial](const FName& Key, const FFireDataTable& Value)
	{
		if (Value.HazardousMaterial == InHazardousMaterial)
		{
			Result = Value;
		}
	};
	
	DT_Fire->ForeachRow<FFireDataTable>(
		TEXT("GetFireSettingsByScenario(...) Expected RowStruct for this DT : FFireDataTable"),
		ForEach);

	return Result;
}

void URW_FireManager::HandleEvent_Impl(USecondTimerExpiredEvent* Event)
{
	if(FireActors.Num() > 0)
	{
		Cast<ARW_FireActor>(FireActors[0])->OnDeath();
	}
}

void URW_FireManager::FireExtinguished(ARW_FireActor* InFireActor)
{
	FireActors.Remove(InFireActor);
	InFireActor->OnFireExtinguished().RemoveDynamic(this, &URW_FireManager::FireExtinguished);
	InFireActor->Destroy();
	
	if(FireActors.Num() <= 0)
	{
		SendEvent<UFireExtinguishedEvent>();
		UE_LOG(LogTemp, Warning, TEXT("UFireExtinguishedEvent"));
	}
}

// acid + water
void URW_FireManager::UncontrollableFire(ARW_FireActor* InFireActor)
{
	InFireActor->OnUncontrollableFire().RemoveDynamic(this, &URW_FireManager::UncontrollableFire);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		SendEvent<UFinishScenarioEvent>(nullptr, EScoringMessageType::SMT_Explosion);
	}, 5.f,false );
}
