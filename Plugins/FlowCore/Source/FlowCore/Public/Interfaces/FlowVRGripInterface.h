#pragma once

#include "CoreMinimal.h"
#include "FlowVRGripInterface.generated.h"

class UFlowVRMotionControllerComponent;

UENUM(BlueprintType)
enum class EFlowVRGripTriggerModeEnum : uint8
{
	/** The grabbed object will be dropped after releasing the trigger. */
	GRIP_TRIGGER_AUTO_DROP UMETA(Displayname="Auto Drop"),

	/** The grabbed object will be dropped after a second grip/release. */
	GRIP_TRIGGER_TOGGLE_DROP UMETA(DisplayName="Toggle Drop"),
};

UENUM(BlueprintType)
enum class EFlowVRGripModeEnum : uint8
{
	/** The grabbed object will be grabbed using normal attachments. */
	GRIP_STATIC UMETA(Displayname="Grip Static"),

	/** The grabbed object will be grabbed using a physics constraint. */
	GRIP_PHYSICS UMETA(DisplayName="Grip Physics"),
};

UENUM(BlueprintType)
enum class EFlowVRGripState : uint8
{
	Open UMETA(Displayname = "Open"),
	CanGrab UMETA(Displayname = "CanGrab"),
	Grab UMETA(Displayname = "Grab"),
	AdvanceGrab UMETA(Displayname = "AdvanceGrab"),
	HoldTool UMETA(Displayname = "HoldTool"),
	HoldTensioner UMETA(Displayname = "HoldTensioner"),
	HoldFastener UMETA(Displayname = "HoldFastener"),
	HoldClipboard UMETA(Displayname = "HoldClipoard"),
	CanInteract UMETA(Displayname = "HoldTool"),
	HoldLaserPointer UMETA(Displayname = "HoldLaserPointer"),
	PointFinger UMETA(Displayname = "PointFinger")
};

USTRUCT(BlueprintType) 
struct FLOWCORE_API FGripInterfaceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Grip Interface Settings")
	bool bDenyGripping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Grip Interface Settings")
	bool bDenyDropping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Grip Interface Settings")
	EFlowVRGripTriggerModeEnum TriggerMode = EFlowVRGripTriggerModeEnum::GRIP_TRIGGER_AUTO_DROP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Grip Interface Settings")
	EFlowVRGripModeEnum GripMode = EFlowVRGripModeEnum::GRIP_STATIC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Grip Interface Settings")
	bool bUseCustomTransform = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Grip Interface Settings")
	FTransform CustomTransform;
};

UINTERFACE(MinimalAPI)
class UFlowVRGripInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOWCORE_API IFlowVRGripInterface
{
	GENERATED_BODY()

public:

	/// Deny gripping. If this is set to true the actor can't be gripped anymore until set the false again.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	bool DenyGrippingFlowVR(UGripMotionControllerComponent* GrippingController);

	/// Deny gripping after triggering grip.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	bool DenyTriggerGripping();

	/// Deny dropping. If this is set to true the actor will not be dropped after releasing until set to false again.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	bool DenyDropping();

	/// Will be called when user grabs an object.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	void OnGripObject(UGripMotionControllerComponent* GrippingController, AActor* GrippedActor);

	/// Will be called when user tried to grab the object.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	void OnTryGripObject(UGripMotionControllerComponent* GrippingController, AActor* GrippedActor);

	/// Will be called when the user releases an object.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	void OnGripObjectRelease(UGripMotionControllerComponent* ReleasingController, AActor* GrippedActor);

	/// Will be used to determine how the gripping trigger should behave.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	void GripTriggerMode(EFlowVRGripTriggerModeEnum& GripTriggerMode);

	/// This specified transformation will be used when flag is set and a grip is happening.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	void GripTriggerTransform(UGripMotionControllerComponent* GrippingController, bool& UseCustomTransform, FTransform& CustomTransform);

	/// Will be used to determine how to do the grabbing. Using normal attachment or a physics constraint.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	void GripMode(EFlowVRGripModeEnum& GripMode);

	/// Disable Physics on Drop. When this is set to 'true' if user drops object it will disable physics else it will use default.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	bool DisablePhysicsOnDrop();
	
	/// This event runs when the user "pauses" this object. Pausing should turn off visibility and functionality of this object.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	void OnObjectPaused();

	/// This event runs when the user "unpauses" this object. Unpausing should turn on visibility and functionality of this object.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Grip Interface")
	void OnObjectUnPaused();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Animation")
	void UpdateHandAnimation(EFlowVRGripState AnimationState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FlowVR | Animation")
	void SetupHandAnimation(AActor* Hand);
};