#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RW_SpillWidget.generated.h"

class UTextBlock;
class UButtonWidget;

UCLASS()
class RAILWRECK_API URW_SpillWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	UTextBlock* Message;
	
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	UButtonWidget* ConfirmButton;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	UButtonWidget* CloseButton;
};
