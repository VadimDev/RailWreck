#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RW_CoughingInterface.generated.h"

UINTERFACE(MinimalAPI)
class URWCoughingInterface : public UInterface
{
	GENERATED_BODY()
};

class RAILWRECK_API IRWCoughingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Life | Setup")
	void EnableCoughing();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Life | Setup")
	void DisableCoughing();
};