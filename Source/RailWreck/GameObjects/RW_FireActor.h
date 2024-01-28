#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RailWreck/Scenario/RW_ScenarioData.h"
#include "RW_FireActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFireExtinguishedDelegate, ARW_FireActor*, FireActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUncontrollableFireDelegate, ARW_FireActor*, FireActor);

UCLASS(Blueprintable)
class RAILWRECK_API ARW_FireActor : public AActor
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	void SetFireShrinkTime(float InFireShrinkTime) { FireShrinkTime = InFireShrinkTime; }
	void SetFirefightingTools(const TArray<EFirefightingTool>& InFirefightingTools) { FirefightingTools = InFirefightingTools; }
	void SetProhibitedFirefightingTools(const TArray<EFirefightingTool>& InProhibitedFirefightingTools) { ProhibitedFirefightingTools = InProhibitedFirefightingTools; }
	FFireExtinguishedDelegate& OnFireExtinguished() { return FireExtinguishedDelegate; }
	FUncontrollableFireDelegate& OnUncontrollableFire() { return UncontrollableFireDelegate; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) float FireShrinkTime = 5.f;
	UPROPERTY(BlueprintReadOnly, Category=Setup) TArray<EFirefightingTool> FirefightingTools;
	UPROPERTY(BlueprintReadOnly, Category=Setup) TArray<EFirefightingTool> ProhibitedFirefightingTools;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadOnly) FFireExtinguishedDelegate FireExtinguishedDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadOnly) FUncontrollableFireDelegate UncontrollableFireDelegate;
};
