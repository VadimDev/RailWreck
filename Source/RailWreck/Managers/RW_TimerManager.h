#pragma once

#include "CoreMinimal.h"
#include "EventSystem/BaseManager.h"
#include "RW_TimerManager.generated.h"

UENUM(BlueprintType)
enum class ETimerType : uint8
{
	TT_None = 0							UMETA(DisplayName="None"),
	TT_Fire								UMETA(DisplayName="Fire"),
	TT_Spill							UMETA(DisplayName="Spill"),
};

UCLASS(Blueprintable)
class RAILWRECK_API URW_TimerManager : public UBaseManager, public FTickableGameObject
{
	GENERATED_BODY()

	URW_TimerManager();
	
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	DECLARE_EVENT_HANDLER(UInitializationEvent);
	DECLARE_EVENT_HANDLER(UFireExtinguishedEvent);
	DECLARE_EVENT_HANDLER(USpillEliminatedEvent);
	DECLARE_EVENT_HANDLER(ULeaveDangerZoneEvent)
	
	UFUNCTION() void LaunchSpillTimer();
	UFUNCTION() void FirstFireTimerFinished();
	UFUNCTION() void SecondFireTimerFinished();
	UFUNCTION() void SpillTimerFinished();
	UFUNCTION() void LaunchTimers();
	UFUNCTION() void LaunchFirstFireTimer();
	UFUNCTION() void LaunchSecondFireTimer();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup | Fire") int32 FirstFireTimer = 240;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup | Fire") int32 SecondFireTimer = 45;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup | Fire") USoundBase* FireEliminationVO;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup | Fire") USoundBase* WarningVO;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup | Spill") int32 SpillTimer = 180;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup | Explosion") float FinishScenarioDelay = 5.f;

private:
	ETimerType CurrentTimerType;
	FTimerHandle TimerHandle;
	FTimerManager TimerManager;
	bool bIsFire;
	bool bIsSpill;
};
