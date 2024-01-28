#pragma once

#include "CoreMinimal.h"
#include "VRCharacter.h"
#include "FlowVRHand.h"
#include "PPE/PPETypes.h"
#include "FlowVRCharacter.generated.h"

class UFlowVRLongHoldComponent;
class UFlowVRInputAdapter;
class AFlowVRCircleProgress;
class AFlowVRWristMenu;
class UDataTable;
class USphereComponent;
class AFlowVRLaser;
class UFlowVRMotionControllerComponent;
class AFlowVRTeleporter;

DECLARE_LOG_CATEGORY_EXTERN(LogFlowVRCharacter, Log, All);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTeleportationActivated, EControllerHand, bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWristMenuActivated, EControllerHand, Hand);

UCLASS(Blueprintable)
class FLOWCORE_API AFlowVRCharacter : public AVRCharacter
{
	GENERATED_BODY()

public:
	
	AFlowVRCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void UpdatePPE(const FPPESetup& PPEItem);
	void UpdatePPEStaticMesh(const EPPEItemType PPEItemType, UStaticMesh* StaticMesh) const;
	void UpdatePPESkeletalMesh(const EPPEItemType PPEItemType, USkeletalMesh* SkeletalMesh);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlowVR")
	float DefaultPlayerHeight;
	
	/********************************* Teleport *********************************/ 
	UFUNCTION(BlueprintCallable) void HandleTeleportLeftPressed();
	UFUNCTION(BlueprintCallable) void HandleTeleportLeftReleased();
	UFUNCTION(BlueprintCallable) void HandleTeleportRightPressed();
	UFUNCTION(BlueprintCallable) void HandleTeleportRightReleased();
	
	UFUNCTION()
	void HandleTeleportLeftUp(float Value);

	UFUNCTION()
	void HandleWristMenuDoubleTap(FKey InputKey);

	UFUNCTION()
	void HandleTeleportLeftRight(float Value);

	UFUNCTION()
	void HandleTeleportRightUp(float Value);
	
	UFUNCTION()
	void HandleTeleportRightRight(float Value);
	
	UFUNCTION()
	void ExecuteTeleportation(EControllerHand Hand);

	// Activate/Deactivate the Teleporter.
	UFUNCTION(BlueprintCallable, Category = "FlowVR")
	void ActivateTeleporter(EControllerHand Hand, bool InActivate);

	UFUNCTION(BlueprintCallable, Category = "FlowVR")
	void NotifyActivateTeleporter(EControllerHand Hand, bool InActivate);

	UFUNCTION(BlueprintCallable, Category = "FlowVR")
	void MulticastActivateTeleporter(EControllerHand Hand, bool InActivate);

	UFUNCTION()
	void SpawnTeleporter(EControllerHand Hand, bool InActivate);

	UPROPERTY(BlueprintReadWrite, Category = "FlowVR")
	bool bLaserBeamActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	FName LeftTeleporterSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	FName RightTeleporterSocket;

	UPROPERTY(BlueprintReadOnly, Category = "FlowVR")
	AFlowVRTeleporter* TeleportControllerLeft;

	UPROPERTY(BlueprintReadOnly, Category = "FlowVR")
	AFlowVRTeleporter* TeleportControllerRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	TSubclassOf<AFlowVRTeleporter> TeleportControllerClass;

	// Allows for disabling of Teleporter via BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	bool bTeleporterEnabled = true;

	FOnTeleportationActivated& OnTeleportationActivated() { return OnTeleportationActivatedEvent; };

	/****************************** WristMenu ****************************/

	UFUNCTION(BlueprintCallable) void HandleActivateWristMenuPressed();
	UFUNCTION(BlueprintCallable) void HandleSelectWristMenuItemPressed();
	UFUNCTION(BlueprintCallable) void HandleActivateWristMenuPressedLeft();
	UFUNCTION(BlueprintCallable) void HandleActivateWristMenuPressedRight();

	// Activate/Deactivate the Wrist Menu.
	UFUNCTION(BlueprintCallable, Category = "FlowVR")
	void ActivateWristMenu(EControllerHand Hand);
	
	UFUNCTION(BlueprintCallable, Category = "FlowVR")
	void ActivateProgressBar(EControllerHand Hand,  bool InActivate);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	TSubclassOf<AFlowVRWristMenu> WristMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	TSubclassOf<AFlowVRCircleProgress> ProgressBarClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	FName WristMenuSocket;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "FlowVR", meta = (AllowPrivateAccess = "true"))
	AFlowVRWristMenu* WristMenu;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "FlowVR", meta = (AllowPrivateAccess = "true"))
	AFlowVRCircleProgress* ProgressBar;

	FVector MirroredWristMenuScale;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated, Category= "FlowVR")
	EControllerHand CurrentWristMenuHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FlowVR", meta = (AllowPrivateAccess = "true"))
	UFlowVRLongHoldComponent* LongHoldComponent;
	
	/****************************** Laser ****************************/ 
	UFUNCTION(BlueprintCallable) void HandleLaserBeamLeftPressed();
	UFUNCTION(BlueprintCallable) void HandleLaserBeamRightPressed();

	UFUNCTION (BlueprintCallable, BlueprintPure)
	void IsLaserActivated (bool& IsActivated, EControllerHand& Hand);

	UFUNCTION()
	void IsOverWidgetUse(EControllerHand Hand, bool InPressed);

	UFUNCTION()
	void PerformIsOverWidgetUse(EControllerHand Hand, bool InPressed);

	UFUNCTION()
	void HandleLaserSelectEvent(EControllerHand Hand, bool Pressed);
	UFUNCTION()
	void MultiHandleLaserSelectEvent(EControllerHand Hand, bool Pressed);
	UFUNCTION()
	void HandleLaserSelect(EControllerHand Hand, bool Pressed);
	
	UFUNCTION()
	void SpawnLaserBeam(EControllerHand Hand, bool InActivate);

	UFUNCTION()
	void SpawnWristMenu();

	UFUNCTION()
	void SpawnProgressBar();

	//Laser Beam Select
	void HandleLaserSelectLeft();
	void HandleLaserReleaseLeft();
	void HandleLaserSelectRight();
	void HandleLaserReleaseRight();

	// Activate/Deactivate the Laser-beam.
	UFUNCTION(BlueprintCallable, Category = "FlowVR")
	void ActivateLaserBeam(EControllerHand Hand, bool InActivate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category ="FlowVR")
	void HandleHandAnimationLaser(EControllerHand Hand);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	FName LeftLaserBeamSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	FName RightLaserBeamSocket;

	UPROPERTY(BlueprintReadOnly, Category = "FlowVR")
	AFlowVRLaser* LaserControllerLeft;

	UPROPERTY(BlueprintReadOnly, Category = "FlowVR")
	AFlowVRLaser* LaserControllerRight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FlowVR")
	bool bLeftLaserBeamIsActiveOnStart;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FlowVR")
	bool bRightLaserBeamIsActiveOnStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	TSubclassOf<AFlowVRLaser> LaserControllerClass;

	UPROPERTY(BlueprintReadOnly)
	bool bActivateLeftLaser;
	
	UPROPERTY(BlueprintReadOnly)
	bool bActivateRightLaser;
	
	// For Windows
	UFUNCTION()
	void ActivateFPSMode(bool Enable);
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	/****************************** Hands ****************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hands")
	TSubclassOf<AFlowVRHand> VRHandClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "Hands")
	AFlowVRHand* VRHandLeft;
	UPROPERTY(BlueprintReadOnly, Category = "Hands")
	AFlowVRHand* VRHandRight;
	
	UFUNCTION(BlueprintCallable, Category = "Hands")
	virtual void GrabRightPressed();
	UFUNCTION(BlueprintCallable, Category = "Hands")
	virtual void GrabLeftPressed();
	UFUNCTION(BlueprintCallable, Category = "Hands")
	virtual void GrabRightReleased();
	UFUNCTION(BlueprintCallable, Category = "Hands")
	virtual void GrabLeftReleased();

	UFUNCTION(BlueprintCallable, Category = "Hands")
	void ResetHandHoldingObject();
	bool HoldToolRight;
	bool HoldToolLeft;

	UFUNCTION(BlueprintCallable)
	void ShowTool(EControllerHand Hand, AActor* Tool);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR")
	FName ToolSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	UStaticMeshComponent* FadeSphere;
	
	float BaseTurnRate;
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FlowVR", Meta=(Tooltip="If checked, Laser, Teleporter and Wrist Menu input handling will be disabled."))
	bool bDisableToolsInputBindings;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FlowVR")
	UDataTable* InputsTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FlowVR", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FlowVR", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Body;

	FVector2D LastTeleportLeftDirectionVector;
	FVector2D LastTeleportRightDirectionVector;
	
	FKey LastPressedKey;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* Hat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* Glasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UStaticMeshComponent* Vest;
	UPROPERTY() USkeletalMesh* CurrentGloves;
	
private:
	
	UFUNCTION()
	void PlayerDestroyed(AActor* Act);
	
	bool bFPSMode;
	FOnTeleportationActivated OnTeleportationActivatedEvent;

	UPROPERTY(Replicated)
	bool bIsOverWidgetUse = false;
};
