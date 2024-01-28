#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "FlowVRLaser.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFlowVRLaser, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLaserBeamActivated, AFlowVRLaser*, Laser, bool, bActivated);

UCLASS(Blueprintable)
class FLOWCORE_API AFlowVRLaser : public AActor
{
	GENERATED_BODY()

public:

	AFlowVRLaser();

	/// Activate/Deactivate the laser beam.
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "FlowVR | Laser")
	void ActivateLaserBeam(bool InActivate);

	/// Check if the laser is over a widget and simulate button press.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Laser")
	bool IsOverWidgetUse(bool InPressed);

	/// Sets the hand type describing which hand is holding it..
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Laser")
	void SetHandType(EControllerHand HandType, bool InIsLocal);

	/// Returns the state if the laser is activated or not.
	UFUNCTION(BlueprintPure, Category = "FlowVR | Laser")
	bool IsActivated() const;

	/// Will be called when an object enters the hover state.
	UFUNCTION(BlueprintNativeEvent, Category = "FlowVR | Laser")
	void OnHoverBegin(const FHitResult& HitResult);

	/// Will be called when an object exits the hover state.
	UFUNCTION(BlueprintNativeEvent, Category = "FlowVR | Laser")
	void OnHoverEnd(const FHitResult& HitResult);

	/// Will be called when the user selects the hovered object.
	UFUNCTION(BlueprintNativeEvent, Category = "FlowVR | Laser")
	void OnSelected(const FHitResult& HitResult);

	/// Will be called when the user unselects the hovered object.
	UFUNCTION(BlueprintNativeEvent, Category = "FlowVR | Laser")
	void OnUnSelected(const FHitResult& HitResult);

	/// Call this method when the user wants to select the hovered object.
	UFUNCTION(BlueprintCallable,  Category = "FlowVR | Laser")
	void TriggerSelect(bool State);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTriggerSelect(bool State, AActor* InteractableObject, const FHitResult& HitResult, APlayerController* Executor);

	UFUNCTION(BlueprintCallable,  Category = "FlowVR | Laser")
	void TriggerHover(bool State, AActor* InteractableObject, const FHitResult& HitResult);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTriggerHover(bool State, AActor* InteractableObject, const FHitResult& HitResult, APlayerController* Executor);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastTriggerHover(bool State, AActor* InteractableObject, const FHitResult& HitResult, APlayerController* Executor);

private:

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	UFUNCTION(Client, Unreliable)
	void ClientTickLaserBeam(float DeltaTime);

	UFUNCTION()
	void OnRep_LaserBeamActive();
	void UpdateStateLaserBeam();

public:

	// This component is used to make child transform easier.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	// This component is used to simulate a laser beam. Its just a mesh that will be scaled.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* LaserBeam;

	// This component is used to draw a sphere to show a hitpoint.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* LaserBeamEndPoint;

	/// This component refers to mesh pointer hold in hand
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* LaserPointerMesh;

	// This component is used to interact with UMG widgets.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UWidgetInteractionComponent* WidgetInteraction;

	/// Will be true if the laser beam is active else false.
	UPROPERTY(BlueprintReadOnly, Transient, ReplicatedUsing=OnRep_LaserBeamActive, Category = "FlowVR | Laser")
	bool bIsLaserBeamActive;

	/// The maximum distance of the laser beam.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Laser")
	float LaserBeamMaxDistance;

	/// Size of the laser beam.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Laser")
	float LaserBeamRadius;

	/// Size of the laser beam endpoint.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Laser")
	float LaserBeamEndpointRadius;

	/// The color of the laser beam.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Laser")
	FLinearColor LaserBeamColor;

	/// The color of the laser beam endpoint.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Laser")
	FLinearColor LaserBeamEndpointColor;

	/// If checked it will fire OnHoverBegin/OnHovered/OnSelected if the laser hovers something.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Laser")
	bool bActivateHovering;

	/// If checked the widget interaction distance will be the same as the laser LaserBeamMaxDistance.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Laser")
	bool bLinkToInteraction;

	UPROPERTY(BlueprintAssignable, Category = "FlowVR | Laser")
	FLaserBeamActivated OnLaserBeamActivated;

	FHitResult HoverHitResult;
	TWeakObjectPtr<class AActor> HitActor;
	UMaterial* LaserBeamMaterial;
	UMaterialInstanceDynamic* LaserBeamMaterialDynamic;
	UMaterialInstanceDynamic* LaserBeamEndpointMaterialDynamic;

private:

	bool bIsLocal;

#if WITH_EDITOR
	// Needed to redraw in realtime inside the editor changes for properties.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
