#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "UI/WristMenu/FlowVRWristMenuDataTableRow.h"
#include "FlowVRWristMenu.generated.h"

struct FFlowVRWristMenuDataTableRow;
class UWidgetComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogFlowVRWristMenu, Log, All);

UCLASS()
class FLOWCORE_API AFlowVRWristMenu : public AActor
{
	GENERATED_BODY()

public:
	AFlowVRWristMenu();

	/// Activate/Deactivate the laser beam. If active nothing will be done.
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "FlowVR | Wrist Menu")
	void ActivateWristMenu(bool InActivate);

	/// Is this Wrist Menu Component activated?
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	bool IsActivated();

	/// You need to call this method to signal that the user did a select action.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	EWristMenuItemCategory SelectMenuItem();

	/// You need to call this method when the user moves his fingers (thumb) on the pad.
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "FlowVR | Wrist Menu")
	void UpdateTrackpadPosition(const FVector2D& position);

	/// Increase selected item. (Use this to do tests with the mouse wheel.)
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void IncreaseSelectedItem(int Amount);

	/// Sets the menu event handlers.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void SetEventHandlers(TArray<UObject*>& InEventHandleObjects);

	/// Calculates the rotation of the pointer arrow using the item index.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void CalculateRotationByItemIndex(const int32 WristMenuItemIndex);

	/// Will be added new item or changed current if found in the DataTableRows array by MenuItemID.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void AddIMenuItem(const FFlowVRWristMenuDataTableRow& InNewItemInfo);
	
protected:

	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void RebuildWristMenu(UDataTable* NewWidgetDataTable);

	/// Resets the data table to the default one.
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void ResetToDefaultTable();

private:

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void TickWristMenu(float DeltaTime);

	/// Create all widgets using the data table.
	void CreateAllWidgets();

	/// Create all widgets.
	void ClearAllWidgets();

	/// Calculates the rotation of the pointer arrow using the input of a controller pad.
	void CalculateRotationFromInput(const FVector2D& Direction);

	/// Parse the menu data table and create all needed widgets.
	bool ParseWidgetDataTable();

	/// Parse the data table rows array and create all needed widgets.
	bool ParseWidgetDataTableRows();

	// Use this function to handle the update of the timeline.
	UFUNCTION()
	void HandleTimeLineFloat(float Value);

	// Use this function to handle the finished event.
	UFUNCTION()
	void HandleTimelineFinished();

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* RadialFloatingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* RadialArrowMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimelineComponent;

	/// This curve will be used when activating/deactivating the wrist menu.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu")
	UCurveFloat* Curve;

	/// Is the wrist menu activated or not.
	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Wrist Menu")
	bool bIsActivated;

	/// The minimum length of the pad pointer that is seen as a valid pointer.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu")
	float ThumbDeadZone;

	/// Holds the current index for normal buttons which the arrow points to.
	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Wrist Menu")
	int CurrentNormalAngleIndex;

	/// Holds the current index for large buttons which the arrow points to.
	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Wrist Menu")
	int CurrentLargeAngleIndex;

	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Wrist Menu")
	int PreviousNormalAngleIndex;

	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Wrist Menu")
	int PreviousLargeAngleIndex;

	/// Holds the angle size of one item inside the wrist menu.
	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Wrist Menu")
	float AnglePerNormalItem;

	/// Holds the angle size of one item inside the wrist menu.
	UPROPERTY(BlueprintReadOnly, Category = "FlowVR | Wrist Menu")
	float AnglePerLargeItem;

	/// Holds the current angle.
	UPROPERTY(BlueprintReadOnly, Transient, Category = "FlowVR | Wrist Menu")
	float CurrentAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu")
	float IconRadialDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu")
	FVector IconRelativeScale;

	/// Holds the current pad direction.
	FVector2D CurrentPadDirection;

	/// Holds the current arrow rotation.
	FRotator CurrentArrowRotation;

	UPROPERTY(Transient)
	UDataTable* WidgetDataTableDefault;

	/// Select the data table that holds information about how the wrist menu is build up.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu", meta = (AllowPrivateAccess = "true"))
	UDataTable* WidgetDataTable;

	/// Select the BP class that is used to render the menu items.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> WristMenuWidgetClass;

	/// Select the BP class that is used to render the Titles of the selected items.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> WristMenuTitleWidgetClass;

	/// Holds the Objects that will handle selection of the menus.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu")
	TArray<UObject*> EventHandleObjects;

	/// This AudioCue will be used when selecting an Menu Item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu")
	USoundCue* SelectAudioCue;

	/// This AudioCue will be used when hovering an Menu Item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu")
	USoundCue* HoverAudioCue;

	// Will be filled with all the needed widget components for the wrist menu.
	TArray<UWidgetComponent*> WidgetComponents;
	TArray<UWidgetComponent*> LargeWidgetComponents;

	// Will be filled with all rows from the data table for easier usage.
	TArray<FFlowVRWristMenuDataTableRow*> DataTableRows;
	TArray<FFlowVRWristMenuDataTableRow*> DataTableRowsNormal;
	TArray<FFlowVRWristMenuDataTableRow*> DataTableRowsLarge;

	UPROPERTY() UWidgetComponent* Title;

	UPROPERTY() UWidgetComponent* CurrentPointedWidgetComponent;
	UPROPERTY() UWidgetComponent* CurrentLargePointedWidgetComponent;

	// This delegate will be fired to be informed when the timeline has a new update.
	FOnTimelineFloat TimelinePostUpdate;

	// This delegate will be fired when the timeline reaches the end.
	FOnTimelineEvent TimelineFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu")
	bool bAlignToPlayerHead;

private:

	int MaxNumberOfNormalButtons;
	int MaxNumberOfLargeButtons;

	FKey LastPressedKey;
};
