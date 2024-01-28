#pragma once

#include "CoreMinimal.h"
#include "VRPlayerController.h"
#include "UObject/Object.h"
#include "RW_PlayerController.generated.h"

// TODO useless

UCLASS()
class RAILWRECK_API ARW_PlayerController : public AVRPlayerController
{
	GENERATED_BODY()

public:
	ARW_PlayerController(); 

	virtual void BeginPlay() override;
};
