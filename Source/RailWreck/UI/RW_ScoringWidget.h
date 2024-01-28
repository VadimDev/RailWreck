#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "RW_ScoringWidget.generated.h"

UCLASS()
class RAILWRECK_API URW_ScoringWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* HeadLabel;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ResultMessage;
};
