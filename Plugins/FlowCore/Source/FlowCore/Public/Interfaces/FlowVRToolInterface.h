#pragma once

#include "CoreMinimal.h"
#include "FlowVRToolInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFlowVRToolInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOWCORE_API IFlowVRToolInterface
{
	GENERATED_BODY()

public:
	// Passes tool data to the tool to provide user configurable data.
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVRTool")
	//void InitializeTool(const FFlowToolData& InToolData);

	// Binds a secondary tool to the primary tool.
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVRTool")
	//void PairSecondaryTool(AActor* SecondaryTool);

	// Wraps the usable functionality
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVRTool")
	void Use(bool InActivate, EControllerHand Hand);

	// Wraps the toggle on and off functionality
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVRTool")
	void ToggleState(EControllerHand Hand); 
};