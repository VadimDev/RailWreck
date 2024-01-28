#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlowVRLongHoldComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnLongHoldReleased);
DECLARE_DYNAMIC_DELEGATE(FOnShortPressReleased);

UENUM(BlueprintType)
enum class EHoldingState : uint8
{
	NONE = 0 UMETA(DisplayName = "None"),
	BUTTON_DOWN = 1<<0 UMETA(DisplayName = "Button Is Down"),
	BUTTON_HOLD = 1<<1 UMETA(DisplayName = "Button Is Held"),
	RELEASED = 1<<2 UMETA(DisplayName = "Button Released"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLOWCORE_API UFlowVRLongHoldComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UFlowVRLongHoldComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category =  "FlowVR")
	void ExecuteLongHold(const FOnLongHoldReleased& LongHoldFunction);
	UFUNCTION(BlueprintCallable, Category =  "FlowVR")
	void StopLongHold();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowVR")
	void IsShortPressValid(bool& IsShortPressValid);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowVR")
	float GetCurrentTimePercentage() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowVR")
	float GetCurrentTime() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowVR")
	float GetNeededTime() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlowVR")
	float NeededTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlowVR")
	float MaxShortPressTime;
	UPROPERTY(BlueprintReadWrite, Category = "FlowVR")
	EHoldingState ButtonState;	

private:
	float CurrentTime;
	FOnLongHoldReleased LongHoldReleasedEvent;
};
