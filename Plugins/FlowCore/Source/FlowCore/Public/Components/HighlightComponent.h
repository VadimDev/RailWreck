#pragma once

#include "CoreMinimal.h"
#include "HighlightComponent.generated.h"

UENUM(BlueprintType)
enum class EHighlightStencilEnum : uint8
{
	HSE_None		= 0		UMETA(DisplayName = "None"),
	HSE_Highlighted = 250	UMETA(DisplayName = "Highlighted"),
	HSE_Selected	= 251	UMETA(DisplayName = "Selected"),
	HSE_Correct		= 252	UMETA(DisplayName = "Correct"),
	HSE_Partial		= 253	UMETA(DisplayName = "Partial"),
	HSE_Incorrect	= 254	UMETA(DisplayName = "Incorrect")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLOWCORE_API UHighlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHighlightComponent();
	EHighlightStencilEnum GetCurrentHighlightState() const { return CurrentHighlightState; }
	
	UFUNCTION(BlueprintCallable, Category = "FlowCore | Highlight")
	void UpdateHighlightValue(const EHighlightStencilEnum NewHighlightState);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "FlowCore | Highlight")
	void MultiUpdateHighlightValue(const EHighlightStencilEnum NewHighlightState);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "FlowCore | Highlight")
	void MultiRefreshHighlight();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = Highlight)
	EHighlightStencilEnum CurrentHighlightState;
	
private:
	TArray<UMaterialInstanceDynamic*> GetDynamicMaterials();
	
	UPROPERTY(EditAnywhere, Category = Highlight)
	TMap<EHighlightStencilEnum, FColor> ColorMap;

	UPROPERTY() TArray<UStaticMeshComponent*> StaticMeshesComponents;
	UPROPERTY() TArray<USkeletalMeshComponent*> SkeletalMeshesComponents;
	UPROPERTY() TArray<UMaterialInstanceDynamic*> DynamicMaterials;
};
