#pragma once

#include "CoreMinimal.h"
#include "VRGripInterface.h"
#include "Components/TextRenderComponent.h"
#include "Interfaces/FlowVRInteractionInterface.h"
#include "Interfaces/FlowVRInteractiveObject.h"
#include "Interfaces/FlowVRToolInterface.h"
#include "UObject/Object.h"
#include "RW_Nozzle.generated.h"

enum class EFirefightingTool : uint8;
class AFlowVRHand;

UCLASS()
class RAILWRECK_API ARW_Nozzle
	: public AActor
	, public IVRGripInterface
	, public IFlowVRStoryObject
	, public IFlowVRInteractionInterface
	, public IFlowVRToolInterface
{
	GENERATED_BODY()

public:
	ARW_Nozzle();

	// Event triggered on the interfaced object when gripped
	virtual void OnGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation) override;
	// Event triggered on the interfaced object when grip is released
	virtual void OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController, const FBPActorGripInformation& GripInformation, bool bWasSocketed) override;
	// Returns if the object is held and if so, which controllers are holding it
	virtual void IsHeld_Implementation(TArray<FBPGripPair>& HoldingControllers, bool& bIsHeld) override;
	// Sets is held, used by the plugin
	virtual void SetHeld_Implementation(UGripMotionControllerComponent* HoldingController, uint8 GripID, bool bIsHeld) override;
	// Grip type to use
	virtual EGripCollisionType GetPrimaryGripType_Implementation(bool bIsSlot) override;

	virtual bool HoldItemByClick_Implementation() override;

	virtual void OnHoverBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor) override;
	virtual void OnHoverEnd_Implementation(const FHitResult& HitResult, APlayerController* Executor) override;

	virtual void ToggleState_Implementation(EControllerHand Hand) override;
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleParticles();
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components)
	UStaticMeshComponent* NozzleMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components)
	UTextRenderComponent* TextRender;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRGripInterface")
	FBPInterfaceProperties VRGripInterfaceSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFlowVRHand* CurrentHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector LeftHandNozzleLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FRotator LeftHandNozzleRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector RightHandNozzleLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FRotator RightHandNozzleRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Setup)
	EFirefightingTool FirefightingToolType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsNozzleOpen = false;
};
