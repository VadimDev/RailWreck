#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FlowCoreInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFlowCoreInterface : public UInterface
{
	GENERATED_BODY()
};

class FLOWCORE_API IFlowCoreInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Manager")
	UBaseManager* GetManagerRef(TSubclassOf<UBaseManager> ManagerClass);
};
