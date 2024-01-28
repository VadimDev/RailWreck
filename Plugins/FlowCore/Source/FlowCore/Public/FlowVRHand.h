#pragma once

#include "CoreMinimal.h"
#include "GripMotionControllerComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Interfaces/FlowVRGripInterface.h"
#include "FlowVRHand.generated.h"

class AFlowVRCharacter;

struct FOverlappingInfo
{
	FOverlappingInfo()
		: OverlapActor(nullptr)
		, OverlapComponent(nullptr)
		, Socket(NAME_None)
	{}

	AActor* OverlapActor;
	UPrimitiveComponent* OverlapComponent;
	FName Socket;
};

USTRUCT(BlueprintType, Category = "Animation")
struct FFingerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Thumb = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Index = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Middle = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Ring = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pinky = 0.0;
};

UENUM(BlueprintType, Category = "Animation")
enum class EGrippableTypeChecker: uint8
{
	Default UMETA(DisplayName = "No Checker"),
	StoryObject UMETA(DisplayName = "Object is using Story Object Interface"),
	StoryActive UMETA(DisplayName = "Object is using Is Object Active"),
	VRGrip UMETA(DisplayName = "Object is using VRGrip Interface"),
};

UCLASS()
class FLOWCORE_API AFlowVRHand : public AActor, public IFlowVRGripInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlowVRHand();

	//Hands component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UGripMotionControllerComponent* MotionController;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* GrabSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* HandMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* LaserMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* ThumbSpline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* IndexSpline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* MiddleSpline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* RingSpline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* PinkySpline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetInteractionComponent* TouchComponent;

	//Grab objects
	UFUNCTION()
	void GrabObject();
	UFUNCTION()
	void ReleaseObject();
	UFUNCTION(BlueprintCallable, Category = "TouchWidget")
	void OnTouchWidget(UWidgetComponent* WidgetComponent, UWidgetComponent* PreviousWidgetComponent);
	UFUNCTION(BlueprintCallable, Category = "TouchWidget")
	void DeactivateTouchCooldown();
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "FlowVR")
	UObject* HandHoldingObject;

	// Laser
	UFUNCTION(BlueprintCallable)
	void ToggleLaser(bool Visible);
	UPROPERTY()
	USoundCue* LaserSound;

	//Advance grab animation
	UFUNCTION(BlueprintCallable, Category = "Animation")
	TArray<FVector> GetRelativePointsArray(USplineComponent* FingerSpline, float Precision);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	float TraceFingerForObstacle(TArray<FVector> RelativePointsArray);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetupFingerTracePoints();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Animation")
	FFingerData FingerData;
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateFingerData();

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool UseCustomAnimation = false;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	bool ChangeHandAnimationNearInteractableObject = false;
	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (EditCondition = "ChangeHandAnimationNearInteractableObject == true", EditConditionHides))
	EGrippableTypeChecker GrippableTypeChecker = EGrippableTypeChecker::Default;
	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (EditCondition = "ChangeHandAnimationNearInteractableObject == true", EditConditionHides))
	bool IsObjectHighlightActive = false;
	bool InteractableObjectNearHand = false;
	AActor* OverlapedActor = nullptr;

	TArray<FVector> ThumbPositionCache;
	TArray<FVector> IndexPositionCache;
	TArray<FVector> MiddlePositionCache;
	TArray<FVector> RingPositionCache;
	TArray<FVector> PinkyPositionCache;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetOverlappingActor(FOverlappingInfo& OverlappingInfo);

	bool TouchComponentCooldownActive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Initialization
	void InitFlowVRHands(EControllerHand MotionSource, AFlowVRCharacter* Character);

	UPROPERTY(BlueprintReadWrite, Category = "Init", meta = (ExposeOnSpawn = "true"))
	EControllerHand HandMotionSource;

	UPROPERTY(BlueprintReadWrite, Category = "Init")
	AFlowVRCharacter* OwningCharacter;
};
