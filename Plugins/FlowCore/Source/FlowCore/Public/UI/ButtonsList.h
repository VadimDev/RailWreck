#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonsList.generated.h"

class UButtonWidget;
class UVerticalBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonClickEventDelegate, FText, ButtonName);

UCLASS(BlueprintType)
class FLOWCORE_API UButtonsListAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> ButtonName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UButtonWidget* ButtonWidgetBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSelectRandomButtonInStart = false;
};

UCLASS()
class FLOWCORE_API UButtonsList : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void OnButtonClicked(FText ButtonName);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UButtonsListAsset* ButtonsListAsset;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* ButtonsContainer;

	FButtonClickEventDelegate OnButtonClickEventDelegate;
};
