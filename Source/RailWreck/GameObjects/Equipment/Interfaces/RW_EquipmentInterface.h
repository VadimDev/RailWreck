#pragma once

#include "CoreMinimal.h"
#include "RailWreck/Scenario/RW_ScenarioData.h"
#include "UObject/Interface.h"
#include "RW_EquipmentInterface.generated.h"

UINTERFACE(MinimalAPI)
class URWEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};

class RAILWRECK_API IRWEquipmentInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment | Setup")
	void SetEquipmentGear(EEquipmentGear NewEquipmentGear);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment | Setup")
	EEquipmentGear GetEquipmentGear() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment | Setup")
	void SetEquipmentItem(EEquipmentItem NewEquipmentItem);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment | Setup")
	EEquipmentItem GetEquipmentItem() const;
};