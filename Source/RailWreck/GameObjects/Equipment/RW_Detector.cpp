#include "RW_Detector.h"
#include "FlowVRHand.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

ARW_Detector::ARW_Detector()
{
	DetectorMesh = CreateDefaultSubobject<UStaticMeshComponent>("DetectorMesh");
	SetRootComponent(DetectorMesh);

	HitComponent = CreateDefaultSubobject<USphereComponent>("HitComponent");
	HitComponent->SetupAttachment(GetRootComponent());
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("DetectorWidget"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	
	VRGripInterfaceSettings.bSimulateOnDrop = true;
	VRGripInterfaceSettings.bIsHeld = false;
	VRGripInterfaceSettings.SlotDefaultGripType = EGripCollisionType::CustomGrip;
	VRGripInterfaceSettings.FreeDefaultGripType = EGripCollisionType::CustomGrip;
}

void ARW_Detector::OnGrip_Implementation(UGripMotionControllerComponent* GrippingController,
	const FBPActorGripInformation& GripInformation)
{
	DisableComponentsSimulatePhysics();
	
	SetActorRelativeLocation(LeftLocation);
	SetActorRelativeRotation(LeftRotation);
	SetActorRelativeScale3D(LeftScale);
}

void ARW_Detector::OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController,
	const FBPActorGripInformation& GripInformation, bool bWasSocketed)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	DetectorMesh->SetSimulatePhysics(true);

	GetWorld()->GetTimerManager().SetTimer(OnDropTimer, this, &ARW_Detector::OnDrop, 2.f, false); 
}

void ARW_Detector::IsHeld_Implementation(TArray<FBPGripPair>& HoldingControllers, bool& bIsHeld)
{
	HoldingControllers = VRGripInterfaceSettings.HoldingControllers;
	bIsHeld = VRGripInterfaceSettings.bIsHeld;
}

void ARW_Detector::SetHeld_Implementation(UGripMotionControllerComponent* HoldingController, uint8 GripID, bool bIsHeld)
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

EGripCollisionType ARW_Detector::GetPrimaryGripType_Implementation(bool bIsSlot)
{
	return EGripCollisionType::CustomGrip;
}

void ARW_Detector::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = GetWorld()->GetGameInstance<URW_GameInstance>();
	DetectorMonitor = Cast<URW_DetectorMonitorWidget>(WidgetComponent->GetWidget());
}

void ARW_Detector::MaterialFound()
{
	AreaCounter++;
	SetMaterialName();
	
	if (DetectorMonitor)
	{
		GameInstance->ScoringData.bDetermineMaterialViaDetector = true;
		
		DetectorMonitor->ScanningInfo->SetVisibility(ESlateVisibility::Hidden);
		DetectorMonitor->FoundInfo->SetVisibility(ESlateVisibility::Visible);
		DetectorMonitor->MaterialType->SetText(MaterialName);
	}
}

void ARW_Detector::MaterialNotFound()
{
	AreaCounter--;
	if(AreaCounter == 0 && DetectorMonitor)
	{
		DetectorMonitor->ScanningInfo->SetVisibility(ESlateVisibility::Visible);
		DetectorMonitor->FoundInfo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ARW_Detector::SetMaterialName()
{
	switch (GameInstance->ScenarioData->HazardousMaterial)
	{
	case EHazardousMaterial::HM_Chlorine: MaterialName = FText::FromString("Chlorine"); break;
	case EHazardousMaterial::HM_CrudeOil: MaterialName = FText::FromString("Crude oil"); break;
	case EHazardousMaterial::HM_LiquidPropaneGas: MaterialName = FText::FromString("Liquid Propane"); break;
	case EHazardousMaterial::HM_SulfuricAcid: MaterialName = FText::FromString("Sulfuric Acid"); break;
	default:
		{
			UE_LOG(LogTemp, Error, TEXT("[GameInstance] Scenario params is not setted up"));
			MaterialName = FText::FromString(""); break;
		}
	}
}
