#pragma once

#include "CoreMinimal.h"
#include "FlowVRWristMenuDataTableRow.h"
#include "FlowVRWristMenuInterface.generated.h"

UENUM(BlueprintType)
enum class EWristMenuInterfaceResponseEnum : uint8
{
	SUCCESS UMETA(DisplayName="Success"),
	FAILED  UMETA(Displayname="Failed")
};

UINTERFACE(BlueprintType)
class FLOWCORE_API UFlowVRWristMenuInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOWCORE_API IFlowVRWristMenuInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flow VR Wrist Menu Interface")
	void OnWristMenuItemSelected(UObject* Owner, const FFlowVRWristMenuDataTableRow& MenuItem);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flow VR Wrist Menu Interface")
	void OnWristMenuItemSelectedRespond(UObject* Responder, EWristMenuInterfaceResponseEnum Response);
};