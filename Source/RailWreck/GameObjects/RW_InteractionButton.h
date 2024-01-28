#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "RW_InteractionButton.generated.h"

class UTextRenderComponent;
class UPointLightComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EButtonType : uint8
{
	SelectScenario = 0		UMETA(DisplayName="Select Scenario"),
	RandomizeScenario		UMETA(DisplayName="Randomize Scenario"),
	ResetScenario			UMETA(DisplayName="Reset Scenario"),
	EnterScenario			UMETA(DisplayName="Enter Scenario")
};

UCLASS(BlueprintType)
class RAILWRECK_API UHubButtonsName : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Hub Buttons Name")
	TMap<EButtonType, FText> Names;
};

UENUM(BlueprintType)
enum class EButtonState : uint8
{
	BS_None = 0				UMETA(DisplayName="None"),
	BS_Default				UMETA(DisplayName="Default"),
	BS_Pressed				UMETA(DisplayName="Pressed"),
	BS_Disabled				UMETA(DisplayName="Disabled")
};

DECLARE_MULTICAST_DELEGATE_OneParam(FButtonPressedDelegate, APlayerController*);
DECLARE_MULTICAST_DELEGATE_OneParam(FButtonReleasedDelegate, APlayerController*);

UCLASS(Blueprintable)
class RAILWRECK_API ARW_InteractionButton : public AActor
{
	GENERATED_BODY()
	
public:	
	ARW_InteractionButton();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent) void ButtonPress();
	UFUNCTION(BlueprintNativeEvent) void ButtonRelease();
	
	UFUNCTION(BlueprintCallable, BlueprintPure) EButtonState GetButtonState() const { return CurrentButtonState; }
	
	UFUNCTION(BlueprintCallable)
	void SetButtonState(EButtonState InButtonState);

	UFUNCTION(BlueprintCallable)
	EButtonType GetButtonType();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UStaticMeshComponent* PedestalComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UStaticMeshComponent* ButtonSupportComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UStaticMeshComponent* ButtonMovingComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UBoxComponent*		  BoxCollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UPointLightComponent* PointLightComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UTextRenderComponent* TextRenderComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup) EButtonType		ButtonType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup) USoundBase*		ButtonPressSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup) UCurveFloat*		MovingButtonCurve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup) UHubButtonsName* HubButtonsName;
	
private:
	UFUNCTION() void OnBeginOverlapButton(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void MovingButtonTimelineProgress(float Value) const;
	UFUNCTION() void MovingButtonTimelineFinished();
	
	bool bIsPressed = false;
	EButtonState CurrentButtonState = EButtonState::BS_Default;
	
	FTimeline MovingButtonTimeLine;
	UPROPERTY() TEnumAsByte<ETimelineDirection::Type> TimelineDirection;
	UPROPERTY() APlayerController* PlayerController;

	FButtonPressedDelegate ButtonPressedDelegate;
	FButtonReleasedDelegate ButtonReleasedDelegate;
};