#include "RW_Nozzle.h"
#include "FlowVRHand.h"
#include "GripMotionControllerComponent.h"
#include "RailWreck/Scenario/RW_ScenarioData.h"

ARW_Nozzle::ARW_Nozzle()
{
	NozzleMesh = CreateDefaultSubobject<UStaticMeshComponent>("NozzleMesh");
	SetRootComponent(NozzleMesh);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	TextRender->SetupAttachment(GetRootComponent());
	
	VRGripInterfaceSettings.bSimulateOnDrop = true;
	VRGripInterfaceSettings.bIsHeld = false;
	VRGripInterfaceSettings.SlotDefaultGripType = EGripCollisionType::CustomGrip;
	VRGripInterfaceSettings.FreeDefaultGripType = EGripCollisionType::CustomGrip;
}

void ARW_Nozzle::OnGrip_Implementation(UGripMotionControllerComponent* GrippingController,
	const FBPActorGripInformation& GripInformation)
{
	DisableComponentsSimulatePhysics();
	
	CurrentHand = Cast<AFlowVRHand>(GrippingController->GetOwner());
	AttachToComponent(CurrentHand->HandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	if (CurrentHand->HandMotionSource == EControllerHand::Left)
	{
		SetActorRelativeLocation(LeftHandNozzleLocation);
		SetActorRelativeRotation(LeftHandNozzleRotation);
	}
	else
	{
		SetActorRelativeLocation(RightHandNozzleLocation);
		SetActorRelativeRotation(RightHandNozzleRotation);
	}
}

void ARW_Nozzle::OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController,
	const FBPActorGripInformation& GripInformation, bool bWasSocketed)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	NozzleMesh->SetSimulatePhysics(true);

	if(FirefightingToolType != EFirefightingTool::FT_GroundMonitor)
	{
		if(IsNozzleOpen) ToggleParticles();
	}
}

void ARW_Nozzle::IsHeld_Implementation(TArray<FBPGripPair>& HoldingControllers, bool& bIsHeld)
{
	HoldingControllers = VRGripInterfaceSettings.HoldingControllers;
	bIsHeld = VRGripInterfaceSettings.bIsHeld;
}

void ARW_Nozzle::SetHeld_Implementation(UGripMotionControllerComponent* HoldingController, uint8 GripID, bool bIsHeld)
{
	if (bIsHeld)
	{
		VRGripInterfaceSettings.HoldingControllers.AddUnique(FBPGripPair(HoldingController, GripID));
		VRGripInterfaceSettings.bWasHeld = true;
		VRGripInterfaceSettings.bIsHeld = VRGripInterfaceSettings.HoldingControllers.Num() > 0;
	}
	else
	{
		VRGripInterfaceSettings.HoldingControllers.Remove(FBPGripPair(HoldingController, GripID));
		VRGripInterfaceSettings.bIsHeld = VRGripInterfaceSettings.HoldingControllers.Num() > 0;
	}
}

void ARW_Nozzle::OnHoverBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor)
{
	if (!TextRender) { return; }
	TextRender->SetHiddenInGame(false);
}

void ARW_Nozzle::OnHoverEnd_Implementation(const FHitResult& HitResult, APlayerController* Executor)
{
	if (!TextRender) { return; }
	TextRender->SetHiddenInGame(true);
}

void ARW_Nozzle::ToggleState_Implementation(EControllerHand Hand)
{
	ToggleParticles();
}

EGripCollisionType ARW_Nozzle::GetPrimaryGripType_Implementation(bool bIsSlot) { return EGripCollisionType::CustomGrip; }

bool ARW_Nozzle::HoldItemByClick_Implementation() { return true; }


