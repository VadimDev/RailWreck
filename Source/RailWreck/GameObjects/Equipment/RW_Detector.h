#pragma once

#include "CoreMinimal.h"
#include "VRGripInterface.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "RailWreck/Core/RW_GameInstance.h"
#include "RailWreck/UI/RW_DetectorMonitorWidget.h"
#include "RW_Detector.generated.h"

UCLASS()
class RAILWRECK_API ARW_Detector : public AActor, public IVRGripInterface
{
	GENERATED_BODY()

	ARW_Detector();

	virtual void BeginPlay() override;
	
public:
	virtual void OnGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation) override;
	virtual void OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController, const FBPActorGripInformation& GripInformation, bool bWasSocketed) override;
	virtual void IsHeld_Implementation(TArray<FBPGripPair>& HoldingControllers, bool& bIsHeld) override;
	virtual void SetHeld_Implementation(UGripMotionControllerComponent* HoldingController, uint8 GripID, bool bIsHeld) override;
	virtual EGripCollisionType GetPrimaryGripType_Implementation(bool bIsSlot) override;
	
	UFUNCTION(BlueprintCallable) void MaterialFound();
	UFUNCTION(BlueprintCallable) void MaterialNotFound();
	UFUNCTION() void SetMaterialName();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRGripInterface")
	FBPInterfaceProperties VRGripInterfaceSettings;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components)   UStaticMeshComponent* DetectorMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)	USphereComponent* HitComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UWidgetComponent* WidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector LeftLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FRotator LeftRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector LeftScale;

	UPROPERTY() URW_GameInstance* GameInstance;
	UPROPERTY() URW_DetectorMonitorWidget* DetectorMonitor;
	
	FText MaterialName;
	int32 AreaCounter = 0;

private:
	void OnDrop() { Destroy(); }
	FTimerHandle OnDropTimer;
};
