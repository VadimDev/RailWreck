#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RW_ScenarioData.generated.h"

UENUM(BlueprintType)
enum class ESpillWidgetType : uint8
{
	SWT_None = 0							UMETA(DisplayName="None"),
	SWT_AKit								UMETA(DisplayName="A-Kit"),
	SWT_BKit								UMETA(DisplayName="B-Kit"),
	SWT_CKit								UMETA(DisplayName="C-Kit"),
	SWT_SlimeBomb							UMETA(DisplayName="Slime Bomb"),
	SWT_OutletValve							UMETA(DisplayName="Outlet Valve"),
	SWT_ContainerValve						UMETA(DisplayName="Container Valve")
};

UENUM(BlueprintType)
enum class EHazardousMaterial : uint8
{
	HM_None = 0							UMETA(DisplayName="None"),
	HM_CrudeOil							UMETA(DisplayName="Crude Oil"),
	HM_Chlorine							UMETA(DisplayName="Chlorine"),
	HM_LiquidPropaneGas					UMETA(DisplayName="Liquid Propane Gas"),
	HM_SulfuricAcid						UMETA(DisplayName="Sulfuric Acid")
};

UENUM(BlueprintType)
enum class EChlorineTrainType : uint8
{
	CT_None = 0							UMETA(DisplayName="None"),
	CT_ChlorineCylinders				UMETA(DisplayName="Chlorine Cylinders"),
	CT_ChlorineContainer				UMETA(DisplayName="Chlorine Container"),
	CT_ChlorineTank						UMETA(DisplayName="Chlorine Tank")
};

UENUM(BlueprintType)
enum class EEquipmentGear : uint8
{
	EG_None = 0							UMETA(DisplayName="None"),
	EG_EncapsulationGear				UMETA(DisplayName="Encapsulation Gear"),
	EG_BunkerGear						UMETA(DisplayName="Bunker Gear"),
};

UENUM(BlueprintType)
enum class EEquipmentItem : uint8
{
	EI_None = 0							UMETA(DisplayName="None"),
	EI_AKit								UMETA(DisplayName="A Kit"),
	EI_BKit								UMETA(DisplayName="B Kit"),
	EI_CKit								UMETA(DisplayName="C Kit"),
	EI_SlimeBomb						UMETA(DisplayName="Slime Bomb")
};

UENUM(BlueprintType)
enum class EApproachDistance : uint8
{
	AD_None = 0							UMETA(DisplayName="None"),
	AD_75Feet							UMETA(DisplayName="75 feet"),
	AD_150Feet							UMETA(DisplayName="150 feet"),
	AD_330Feet							UMETA(DisplayName="330 feet")
};

UENUM(BlueprintType)
enum class EFirefightingTool : uint8
{
	FT_None = 0							UMETA(DisplayName="None"),
	FT_FoamNozzle						UMETA(DisplayName="Foam Nozzle"),
	FT_WaterNozzle						UMETA(DisplayName="Water Nozzle"),
	FT_GroundMonitor					UMETA(DisplayName="Ground Monitor")
};

USTRUCT(BlueprintType)
struct RAILWRECK_API FFireDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EHazardousMaterial HazardousMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float FireShrinkTime = 5.f; // water + acid = explosion 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TArray<EFirefightingTool> FirefightingTools;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TArray<EFirefightingTool> ProhibitedFirefightingTools;
};

USTRUCT(BlueprintType)
struct RAILWRECK_API FEquipmentGearDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EHazardousMaterial HazardousMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) bool bIsFire;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EEquipmentGear EquipmentGear;
};

USTRUCT(BlueprintType)
struct RAILWRECK_API FScoringData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	bool bGeneralSuccess;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	bool bIsCorrectApproachDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	bool bCorrectEquipment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	bool bFireExtinguished;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bSpillEliminated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bDetermineMaterialViaDetector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bDecontaminationTentUsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bFirstTimerExpired;
};

UENUM(BlueprintType)
enum class EScoringMessageType : uint8
{
	SMT_Success							UMETA(DisplayName="Success"),
	SMT_Fail							UMETA(DisplayName="Fail"),
	SMT_IncorrectEquipment				UMETA(DisplayName="Incorrect Equipment"),
	SMT_Explosion						UMETA(DisplayName="Explosion"),
	SMT_SecondFireTimerExpired			UMETA(DisplayName="Second Fire Timer Expired"),
	SMT_SpillTimerExpired				UMETA(DisplayName="Spill Timer Expired"),
	SMT_LeftTheTraining					UMETA(DisplayName="Left The Training")
};

UCLASS(BlueprintType)
class RAILWRECK_API UScenarioData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite) EHazardousMaterial HazardousMaterial = EHazardousMaterial::HM_None;
	UPROPERTY(BlueprintReadWrite) bool IsSpill = false;
	UPROPERTY(BlueprintReadWrite) bool IsDay   = false;
	UPROPERTY(BlueprintReadWrite) bool IsFire  = false;
	
	UPROPERTY(BlueprintReadWrite) EChlorineTrainType ChlorineTrainType  = EChlorineTrainType::CT_ChlorineCylinders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EApproachDistance ApproachDistance = EApproachDistance::AD_75Feet;
};
