#pragma once

#include "CoreMinimal.h"
#include "FlowVRCharacter.h"
#include "RailWreck/GameObjects/RW_CoughingInterface.h"
#include "RailWreck/GameObjects/Equipment/Interfaces/RW_EquipmentInterface.h"
#include "UI/WristMenu/FlowVRWristMenuDataTableRow.h"
#include "UI/WristMenu/FlowVRWristMenuInterface.h"
#include "RW_PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWristMenuExecuted, FString, Option, ARW_PlayerCharacter*, PlayerController);

UCLASS()
class RAILWRECK_API ARW_PlayerCharacter : public AFlowVRCharacter, public IRWEquipmentInterface, public IRWCoughingInterface, public IFlowVRWristMenuInterface
{
	GENERATED_BODY()

	void MakeCough() const;

public:
	virtual void SetEquipmentGear_Implementation(EEquipmentGear NewEquipmentGear) override;
	virtual EEquipmentGear GetEquipmentGear_Implementation() const override;
	
	virtual void SetEquipmentItem_Implementation(EEquipmentItem NewEquipmentItem) override;
	virtual EEquipmentItem GetEquipmentItem_Implementation() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Equipment") TMap<EEquipmentGear,FPPESetup> Gloves;

	virtual void EnableCoughing_Implementation() override;
	virtual void DisableCoughing_Implementation() override;
	
	virtual void OnWristMenuItemSelected_Implementation(UObject* WristMenuObject, const FFlowVRWristMenuDataTableRow& MenuItem) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Wrist Menu")
	FOnWristMenuExecuted WristMenuDispatcher;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|WristMenu") FFlowVRWristMenuDataTableRow WristMenuItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|WristMenu") TMap<EEquipmentItem,FString> Titles;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup") USoundBase* CoughSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup") float CoughDelayTime = 2.f;
	
private:
	EEquipmentGear EquipmentGear;
	EEquipmentItem EquipmentItem;

	FTimerHandle CoughTimerHandler;
};
