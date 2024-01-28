#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/FlowVRInteractionInterface.h"
#include "RW_TentInBag.generated.h"

class UWidgetComponent;
class UTextRenderComponent;
class USphereComponent;

UCLASS()
class RAILWRECK_API ARW_TentInBag : public AActor, public IFlowVRInteractionInterface
{
	GENERATED_BODY()

public:
	ARW_TentInBag();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnHoverBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor) override;
	virtual void OnHoverEnd_Implementation(const FHitResult& HitResult, APlayerController* Executor) override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnCloseClickedEvent();
	
	UFUNCTION() void OnCloseButtonClicked(FText ButtonName);
	UFUNCTION() void OnConfirmButtonClicked(FText ButtonName);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UStaticMeshComponent* EquipmentMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) USphereComponent* SphereCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UTextRenderComponent* TextRender;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components) UWidgetComponent* TentWidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bWidgetOpened;
};
