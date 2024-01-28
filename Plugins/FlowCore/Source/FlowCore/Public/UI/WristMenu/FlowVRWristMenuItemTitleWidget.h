#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FlowVRWristMenuItemTitleWidget.generated.h"

class UTextBlock;

UCLASS()
class FLOWCORE_API UFlowVRWristMenuItemTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="FlowVR | Wrist Menu")
	void SetTitleText(FText Title);

	UFUNCTION(BlueprintCallable, Category = "FlowVR | Wrist Menu")
	void SetTitleColor(FLinearColor color);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlowVR | Wrist Menu", meta = (BindWidget))
	UTextBlock* HighlightedItemTitle;
};
