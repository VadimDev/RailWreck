#include "RW_TimerManager.h"
#include "Events/FinishScenarioEvent.h"
#include "Events/FireExtinguishedEvent.h"
#include "Events/FirstTimerExpiredEvent.h"
#include "Events/LeaveDangerZoneEvent.h"
#include "Events/SecondTimerExpiredEvent.h"
#include "Events/SpillEliminatedEvent.h"
#include "EventSystem/Events/Experience/InitializationEvent.h"
#include "Kismet/GameplayStatics.h"
#include "RailWreck/Core/RW_GameInstance.h"

URW_TimerManager::URW_TimerManager()
{
	DEFINE_EVENT_HANDLER(UInitializationEvent);
	DEFINE_EVENT_HANDLER(UFireExtinguishedEvent);
	DEFINE_EVENT_HANDLER(USpillEliminatedEvent);
	DEFINE_EVENT_HANDLER(ULeaveDangerZoneEvent);
}

void URW_TimerManager::Tick(float DeltaTime)
{
#if UE_EDITOR
	if (TimerHandle.IsValid())
	{
		const float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle);
		const FString MessageLog = FString::Printf(TEXT("Remaining Time - %f"), RemainingTime);
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, MessageLog);
	}
#endif
}

TStatId URW_TimerManager::GetStatId() const
{
	return GetStatID();
}

void URW_TimerManager::HandleEvent_Impl(UInitializationEvent* Event)
{
	const auto GameInstance = GetWorld()->GetGameInstance<URW_GameInstance>();
	const auto ScenarioData = GameInstance->ScenarioData;

	bIsFire = ScenarioData->IsFire;
	bIsSpill = ScenarioData->IsSpill;
}

void URW_TimerManager::HandleEvent_Impl(UFireExtinguishedEvent* Event)
{
	bIsFire = false;

	if (CurrentTimerType == ETimerType::TT_Fire)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		if (bIsSpill)
		{
			LaunchSpillTimer();
		}
	}
}

void URW_TimerManager::HandleEvent_Impl(USpillEliminatedEvent* Event)
{
	bIsSpill = false;
	if (CurrentTimerType == ETimerType::TT_Spill)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void URW_TimerManager::HandleEvent_Impl(ULeaveDangerZoneEvent* Event)
{
	const auto GameInstance = GetWorld()->GetGameInstance<URW_GameInstance>();
	if(GameInstance->ScenarioFinishingType != EScoringMessageType::SMT_LeftTheTraining)
	{
		if (CurrentTimerType == ETimerType::TT_None)
		{
			LaunchTimers();
		}
	}
}

void URW_TimerManager::LaunchSpillTimer()
{
	CurrentTimerType = ETimerType::TT_Spill;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URW_TimerManager::SpillTimerFinished, SpillTimer);
}

void URW_TimerManager::FirstFireTimerFinished()
{
	SendEvent<UFirstTimerExpiredEvent>(nullptr);
	LaunchSecondFireTimer();
}

void URW_TimerManager::SecondFireTimerFinished()
{
	SendEvent<USecondTimerExpiredEvent>(nullptr);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		SendEvent<UFinishScenarioEvent>(nullptr, EScoringMessageType::SMT_SecondFireTimerExpired);
	}, FinishScenarioDelay,false );
}

void URW_TimerManager::SpillTimerFinished()
{
	SendEvent<UFinishScenarioEvent>(nullptr, EScoringMessageType::SMT_SpillTimerExpired);
}

void URW_TimerManager::LaunchTimers()
{
	if (bIsFire)
	{
		LaunchFirstFireTimer();
	}
	else if (bIsSpill)
	{
		LaunchSpillTimer();
	}
}

void URW_TimerManager::LaunchFirstFireTimer()
{
	CurrentTimerType = ETimerType::TT_Fire;
	const UWorld* ThisWorld = GetWorld();
	ThisWorld->GetTimerManager().SetTimer(TimerHandle, this, &URW_TimerManager::FirstFireTimerFinished, FirstFireTimer);

	if (ensure(FireEliminationVO))
	{
		UGameplayStatics::GetPlayerController(this, 0)->ClientPlaySound(FireEliminationVO);
	}
}

void URW_TimerManager::LaunchSecondFireTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URW_TimerManager::SecondFireTimerFinished, SecondFireTimer);
	if (ensure(WarningVO))
	{
		UGameplayStatics::GetPlayerController(this, 0)->ClientPlaySound(WarningVO);
	}
}