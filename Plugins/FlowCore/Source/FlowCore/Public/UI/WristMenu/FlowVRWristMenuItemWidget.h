#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "FlowVRWristMenuItemWidget.generated.h"

UCLASS()
class FLOWCORE_API UFlowVRWristMenuItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/// Sets the texture for the button.
	UFUNCTION(BlueprintCallable, Category="FlowVR | Wrist Menu")
	void SetButtonTexture(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void SetButtonIcon(UTexture2D* Texture);

	//SetButtonAngle
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void SetButtonAngle(float Angle);

	//GetButtonAngle
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	float GetButtonAngle() const;

	//Set Button Highlight
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void SetButtonHighlight(FLinearColor TintColor);

	//Set Button Scale
	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void SetButtonScale(FVector2D Scale);

	/// Use BindWidget to inform UBT that we want to control the widget form the c++ level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FlowVR | Wrist Menu", meta = (BindWidget))
	UImage* ImageButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FlowVR | Wrist Menu", meta = (BindWidget))
	UImage* ButtonIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu", meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
};