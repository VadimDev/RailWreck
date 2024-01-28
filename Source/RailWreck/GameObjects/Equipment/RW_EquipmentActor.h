#pragma once

#include "CoreMinimal.h"
#include "VRGripInterface.h"
#include "GameFramework/Actor.h"
#include "Interfaces/FlowVRInteractionInterface.h"
#include "RailWreck/Scenario/RW_ScenarioData.h"
#include "RW_EquipmentActor.generated.h"

class ARW_PlayerCharacter;
enum class EEquipmentGear : uint8;
class UMotionControllerComponent;
class USphereComponent;
class UTextRenderComponent;

UCLASS()
class RAILWRECK_API ARW_EquipmentActor : public AActor, public IVRGripInterface, public IFlowVRInteractionInterface
{
	GENERATED_BODY()

public:
	ARW_EquipmentActor();

	// Sets the Deny Gripping variable on the FBPInterfaceSettings struct
	UFUNCTION(BlueprintCallable, Category = "VRGripInterface")
	void SetDenyGripping(bool bDenyGripping);

	// Set up as deny instead of allow so that default allows for gripping
	// The GripInitiator is not guaranteed to be valid, check it for validity
	virtual bool DenyGripping_Implementation(UGripMotionControllerComponent * GripInitiator = nullptr) override;

	// Sets the grip priority on the FBPInterfaceSettings struct
	UFUNCTION(BlueprintCallable, Category = "VRGripInterface")
	void SetGripPriority(int NewGripPriority);

	// Should this object simulate on drop
	virtual bool SimulateOnDrop_Implementation() override;

	// What grip stiffness and damping to use if using a physics constraint
	virtual void GetGripStiffnessAndDamping_Implementation(float& GripStiffnessOut, float& GripDampingOut) override;

	// Event triggered on the interfaced object when gripped
	virtual void OnGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation) override;
	// Event triggered on the interfaced object when grip is released
	virtual void OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController, const FBPActorGripInformation& GripInformation, bool bWasSocketed = false) override;

	virtual void OnHoverBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor) override;
	virtual void OnHoverEnd_Implementation(const FHitResult& HitResult, APlayerController* Executor) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRGripInterface")
	FBPInterfaceProperties VRGripInterfaceSettings;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void PlayEquipmentSound() const;
	
	FTransform DefaultPosition;
	bool bIsDropped;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void LaunchTimer();
	void ResetTimer();
	
	UFUNCTION()
	void ForceDropObjectAndResetPosition(bool SimpleDrop);
	
	UPROPERTY(BlueprintReadOnly) UGripMotionControllerComponent* MotionController;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UStaticMeshComponent* EquipmentMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) USphereComponent* SphereCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UTextRenderComponent* EquipmentName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) EEquipmentItem EquipmentItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) EEquipmentGear EquipmentGear;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) float TimeToReset = 3.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) float DistanceToReset = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) USoundBase* EquipmentSound;
};
