#pragma once

#include "Blueprint/UserWidget.h"
#include "UIData.generated.h"

class UBasicWidget;

UCLASS(BlueprintType)
class FLOWCORE_API UBasicWidgetInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UBasicWidgetInfo(): bPredefinedTransform(false)
	{
		WidgetClass = UUserWidget::StaticClass();
	}

	UFUNCTION(BlueprintCallable, Category = "Update")
	virtual void Update(const UBasicWidgetInfo* InNewData)
	{
		WidgetClass = InNewData->WidgetClass;
		Heading = InNewData->Heading;
		bExitMenuButton = InNewData->bExitMenuButton;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasicWidget> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Heading = FText::FromString(TEXT("Heading"));
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bExitMenuButton = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRotate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bForcedExecuteOnCloseCallback = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ClosingDistanceCm = -1.0f;

	UPROPERTY() bool bPredefinedTransform;
	UPROPERTY() FVector Location;
	UPROPERTY() FRotator Rotator;
};

UCLASS(BlueprintType)
class FLOWCORE_API UBasicResponseData : public UObject
{
	GENERATED_BODY()

public:
	UBasicResponseData() { Data = this; }
	~UBasicResponseData()
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected option :"));
	}

	bool NeedConfirm() const { return NeedShowConfirmation; }
	void SetNeedConfirmation(bool Value) { NeedShowConfirmation = Value; };

private:
	UPROPERTY()	UBasicResponseData* Data;
	UPROPERTY() bool NeedShowConfirmation = false;
};