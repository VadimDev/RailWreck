#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/ButtonWidget.h"
#include "UI/UIData.h"
#include "BasicWidget.generated.h"

DECLARE_DELEGATE_OneParam(FExitMenuEventDelegate, UBasicResponseData*);

UCLASS()
class FLOWCORE_API UBasicWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Init(UBasicWidgetInfo* WidgetInfo);
	
	UFUNCTION(Category = "Buttons")
	virtual void OnClickExitMenuEvent(FText ButtonName);

	void ProcessSquaredDistanceToPlayer(const float SquaredDistanceToPlayerCm, const float DeltaSec);
	virtual void PlayerWentAwayHandler(const float SquaredDistanceToPlayerCm) { PlayerWentAway(); }
	
	bool IsForcedToExecuteCloseCallback() const { return bForceExecuteCloseCallback; }
	
	FExitMenuEventDelegate OnExitMenuEventDelegate;

protected:

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="PlayerWentAwayHandler"))
	void PlayerWentAway();
	
	UPROPERTY(BlueprintReadOnly, Category = "Buttons", meta = (BindWidget)) UButtonWidget* ExitMenuButton;
	UPROPERTY(BlueprintReadWrite) FText Heading = FText::FromString(TEXT("Heading"));
	UPROPERTY(BlueprintReadWrite) float MaxDistanceCalculationDelaySec = 5.0f;

private:
	bool bForceExecuteCloseCallback = false;
	bool bIsClosingByDistance = false;
	int32 MaxSquaredClosingDistance = -1;
	float DistanceCalculationDelay = 0.0f;
};
