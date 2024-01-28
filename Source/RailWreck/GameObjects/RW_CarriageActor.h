#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RW_CarriageActor.generated.h"

class UTextRenderComponent;

UCLASS(Blueprintable)
class RAILWRECK_API ARW_CarriageActor : public AActor
{
	GENERATED_BODY()

public:	
	ARW_CarriageActor();
	void UpdateTrainIDText(const FString& TrainIDText);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)	UStaticMeshComponent* CarriageMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)	UTextRenderComponent* TrainID1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)	UTextRenderComponent* TrainID2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup) FString TrainIDText = "AAAA 000000";
};
