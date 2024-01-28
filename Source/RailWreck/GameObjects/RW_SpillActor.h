#pragma once

#include "CoreMinimal.h"
#include "Equipment/RW_EquipmentActor.h"
#include "GameFramework/Actor.h"
#include "Interfaces/FlowVRInteractionInterface.h"
#include "RW_SpillActor.generated.h"

class URW_ScenarioManager;
class URW_SpillWidget;
class UWidgetComponent;
enum class ESpillWidgetType : uint8;
class UHighlightComponent;

UCLASS()
class RAILWRECK_API ARW_SpillActor : public AActor, public IFlowVRInteractionInterface
{
	GENERATED_BODY()

public:
	ARW_SpillActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void FixTheSpill(ESpillWidgetType WidgetType);

	UFUNCTION(BlueprintImplementableEvent)
	void FixTheSpillEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void ScenarioWithoutSpillEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void OnActorPressEvent();
	
	virtual void OnHoverBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor) override;
	virtual void OnHoverEnd_Implementation(const FHitResult& HitResult, APlayerController* Executor) override;
	virtual void OnPressBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor) override;
	virtual bool DisableHoverEvents_Implementation() override;
	virtual bool DisablePressEvents_Implementation() override;
	
	EEquipmentItem GetNeededEquipmentItem() const { return NeededEquipmentItem; }
	ESpillWidgetType GetSpillWidgetType() const { return SpillWidgetType; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) UStaticMeshComponent* SpillMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) UHighlightComponent* HighlightComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) USoundBase* EliminationSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setup) ESpillWidgetType SpillWidgetType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setup) EEquipmentItem NeededEquipmentItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) bool bIsEliminated;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) UWidgetComponent* SpillWidgetComponent;
	
	FVector2d WidgetDefaultSize{FVector2d{1920,1080}};
	
	UPROPERTY()
	URW_ScenarioManager* ScenarioManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsActorUsed = false;
};
