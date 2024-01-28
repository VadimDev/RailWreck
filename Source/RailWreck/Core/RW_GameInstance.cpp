#include "RW_GameInstance.h"

void URW_GameInstance::Init()
{
	Super::Init();

	ScenarioData = NewObject<UScenarioData>(this, UScenarioData::StaticClass());
	CreateEmptyScenario();
}

void URW_GameInstance::CreateEmptyScenario() const
{
	ScenarioData->HazardousMaterial = EHazardousMaterial::HM_CrudeOil;
	ScenarioData->IsSpill = false;
	ScenarioData->IsDay = true;
	ScenarioData->IsFire = false;
	ScenarioData->ChlorineTrainType = EChlorineTrainType::CT_None;
	ScenarioData->ApproachDistance = EApproachDistance::AD_75Feet;
}
