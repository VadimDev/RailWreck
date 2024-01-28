#pragma once

#include "CoreMinimal.h"
#include "FlowVRInteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFlowVRInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOWCORE_API IFlowVRInteractionInterface
{
	GENERATED_BODY()

public:

	/// Will be fired when object gets hovered.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Interaction")
	void OnHoverBegin(const FHitResult& HitResult, APlayerController* Executor);

	/// Will be fired when object leaves the hovered state.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Interaction")
	void OnHoverEnd(const FHitResult& HitResult, APlayerController* Executor);

	/// Will be fired when object gets pressed.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Interaction")
	void OnPressBegin(const FHitResult& HitResult, APlayerController* Executor);

	/// Will be fired when object gets released.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Interaction")
	void OnPressEnd(const FHitResult& HitResult, APlayerController* Executor);

	/// If this member returns false the hovering will be disabled.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Interaction")
	bool DisableHoverEvents();

	/// If this member returns false the press event will be disabled.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Interaction")
	bool DisablePressEvents();
};