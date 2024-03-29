﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowVRTeleporter.generated.h"

class USplineComponent;
class UStaticMeshComponent;
class UMaterial;
class UMaterialInstance;
class UGripMotionControllerComponent;
class UNavigationSystemV1;
class USplineMeshComponent;
class UVRBaseCharacterMovementComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogFlowVRTeleporter, Log, All);

UCLASS()
class FLOWCORE_API AFlowVRTeleporter : public AActor
{
	GENERATED_BODY()

public:

	AFlowVRTeleporter();

	/// Activate/Deactivate the Teleporter Controller. If active nothing will be done.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Teleporter")
	void ActivateTeleporter(bool InActivate);

	/// This method is used for pad rotation. Pass the motion controller pad axis here.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Teleporter")
	void UpdateMotionControllerRotation(const FVector2D& Direction);

	/// Returns the Teleport destination.
	UFUNCTION(BlueprintPure, Category = "FlowVR | Teleporter")
	FVector GetTeleportDestination(const FVector& OriginalLocation);

	/// Returns the Teleport transformation which is Location and Orientation.
	UFUNCTION(BlueprintPure, Category = "FlowVR | Teleporter")
	void GetTeleportationTransform(FVector& Location, FVector& ForwardVector);

	/// Sets the motion controller that controls this Teleport.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Teleporter")
	void SetMotionController(UGripMotionControllerComponent* InMotionController);

	/// True if the Teleporter is active else False.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Teleporter")
	bool IsActivated();

	/// Execute the teleportation.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Teleporter")
	void ExecuteTeleportation(UVRBaseCharacterMovementComponent* MovementComponent, 
														const FTransform& ActorTransform, 
														const FVector& VRLocation, 
														const FRotator& ActorRotation);

private:

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	/// Use use NavVolume to find a destination for Teleport.
	bool TraceTeleportDestination(TArray<FVector>& TracePoints, FVector& TraceLocation, FVector& NavMeshLocation);

	/// Clear the arc.
	void ClearArc();

	/// Update the arc spline.
	void UpdateArcSpline(TArray<FVector>& SplinePoints, bool ValidLocationFound);

	/// Update the endpoint of the arch spline.
	void UpdateArcEndpoint(const FVector& NewLocation, bool ValidLocationFound);

	/// Update the Teleport cylinder.
	void UpdateTeleportCylinder(const FVector& NewLocation, bool ValidLocationFound);

	/// Ticks the Teleporter. Don't use this method manually. It is used inside TickComponent.
	void TickTeleporter(float DeltaTime);

	/// Calculate the FRotator from a pad of a controller.
	FRotator GetRotationFromInput(const FVector2D& Direction);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	USplineComponent* ArcSplineComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* ArcEndPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* TeleportCylinder;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Ring;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Teleporter", Meta = (AllowPrivateAccess = "true"))
	UStaticMesh* ArcSplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Teleporter", Meta = (AllowPrivateAccess = "true"))
	UMaterial* ArcSplineMaterial;

	UPROPERTY(Replicated)
	UGripMotionControllerComponent* MotionController;

	/// Holds the state if the teleporter is active or not.
	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Teleporter")
	bool bIsTeleporterActive;

	/// Holds the state if the last teleport location is valid or not.
	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Teleporter")
	bool bIsValidTeleportDestination;

	/// If set to 'true' it will draw the arrow that points to the direction of the controller pad location.
	UPROPERTY(EditAnywhere, Category = "FlowVR | Teleporter")
	bool bDrawTeleportArrow;

	/// If set to 'true' it will draw the arc spline that is used to show the direction of the teleport location.
	UPROPERTY(EditAnywhere, Category = "FlowVR | Teleporter")
	bool bDrawArcSpline;

	/// If set to 'true' it will draw the cylinder that is used to show the area of the teleport location.
	UPROPERTY(EditAnywhere, Category = "FlowVR | Teleporter")
	bool bDrawTeleportCylinder;

	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Teleporter")
	FRotator TeleportRotation;

	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Teleporter")
	FRotator PadRotation;

	/// This value is used to check when to create a direction arrow of the controller pad.
	UPROPERTY(EditAnywhere, Category = "FlowVR | Teleporter")
	float ThumbDeadZone;

	/// Set this to higher values to make the teleporter teleport more far away.
	UPROPERTY(EditAnywhere, Category = "FlowVR | Teleporter")
	float TeleportLaunchVelocity;

	UPROPERTY(EditAnywhere, Category = "FlowVR | Teleporter")
	float FadeInDuration;

	UPROPERTY(EditAnywhere, Category = "FlowVR | Teleporter")
	float FadeOutDuration;

	UPROPERTY(EditAnywhere, Category = "FlowVR | Teleporter")
	FLinearColor TeleportFadeColor;

private:

	TArray<USplineMeshComponent*> SplineMeshComponents;
	UNavigationSystemV1* NavigationSystem;
	FVector TimedLocation;
	FRotator TimedRotation;
	UVRBaseCharacterMovementComponent* MovementComponent;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
