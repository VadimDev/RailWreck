#pragma once

#include "CoreMinimal.h"
#include "FlowVRInteractiveObject.generated.h"

// Struct used in Fadable component
USTRUCT(BlueprintType)
struct FTransparentMaterialData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* MeshRef;

	UPROPERTY(BlueprintReadWrite)
	TArray<UMaterialInstance*> MI_OpaqueMaterials;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<UMaterialInstance*> MI_TransparentMaterials;
	
};

UINTERFACE(MinimalAPI)
class UFlowVRStoryObject : public UInterface
{
	GENERATED_BODY()
};

class FLOWCORE_API IFlowVRStoryObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Story")
	void ObjectActivated();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Story")
	void ObjectDeactivated();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Story")
	bool IsObjectActive();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fadable")
	TArray<FTransparentMaterialData> GetActorTransparentMaterialData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hint")
	void UpdateObjectHighlight(bool Activate);
	
	/// Determines if you need to hold object by click or holding button
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grip Behavior")
	bool HoldItemByClick();
};
