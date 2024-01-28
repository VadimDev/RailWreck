#include "RW_FireActor.h"
#include "RailWreck/Core/RW_GameInstance.h"

void ARW_FireActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(!GetWorld()->GetGameInstance<URW_GameInstance>()->ScenarioData->IsFire)
	{
		Destroy();
	}
}
