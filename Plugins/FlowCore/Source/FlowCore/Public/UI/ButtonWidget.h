#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWidget.generated.h"

class UTextBlock;
class UButton;
class UBorder;

UENUM()
enum class ButtonType
{
	BT_None,
	BT_CheckButton
};

class UButtonWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickEventDelegate, FText, ButtonName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FClickEventPtrDelegate, FText, ButtonName, int32, UID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickEventSelfPtrDelegate, UButtonWidget*, ButtonObj);

UCLASS()
class FLOWCORE_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void SetButtonName(const FText& Name);

	UFUNCTION(BlueprintCallable)
	const FText& GetButtonName();

	// Change state the button on true or false
	UFUNCTION(BlueprintCallable)
	virtual void ChangeButtonState(const bool bState);

	UFUNCTION(BlueprintCallable)
	bool GetState() const { return bCurrentState; } ;

	ButtonType GetType() const { return Type; };

private:
	// Change the button color when button selected or deselected
	void SwitchButtonColor(const bool bState) const;

	UFUNCTION()
	void ButtonReleased();

	UFUNCTION()
	void ButtonHovered();

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegate")
	FClickEventDelegate OnClickEventDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegate")
	FClickEventPtrDelegate OnClickEventPtrDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegate")
	FClickEventSelfPtrDelegate OnClickEventSelfPtrDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* ButtonBounds;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Button;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* BtnName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonName = FText::FromString(TEXT("Text"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DefaultButtonColor = FLinearColor(0.91f, 0.22f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SelectedButtonColor = FLinearColor(0.25f, 0.6f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SelectSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* HoverSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSelectable = false;

	ButtonType Type;

private:
	bool bCurrentState;
};
