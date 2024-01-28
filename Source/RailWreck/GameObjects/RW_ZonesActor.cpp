#include "RW_ZonesActor.h"
#include "RW_CoughingInterface.h"
#include "Components/SphereComponent.h"
#include "Equipment/Interfaces/RW_EquipmentInterface.h"
#include "EventSystem/EventLogService.h"
#include "Kismet/GameplayStatics.h"
#include "RailWreck/Core/RW_ExpGameMode.h"
#include "RailWreck/Core/RW_GameInstance.h"
#include "RailWreck/Managers/RW_ScenarioManager.h"
#include "RailWreck/Managers/Base/RW_ExperienceManager.h"
#include "RailWreck/Managers/Events/FinishScenarioEvent.h"
#include "RailWreck/Managers/Events/LeaveDangerZoneEvent.h"
#include "RailWreck/Managers/Events/SetEquipmentEvent.h"

ARW_ZonesActor::ARW_ZonesActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	USceneComponent* DefaultRootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
	SetRootComponent(DefaultRootComponent);

	SafeZoneComponent = CreateDefaultSubobject<USphereComponent>("SafeZoneComponent");
	SafeZoneComponent->SetupAttachment(GetRootComponent());
	SafeZoneComponent->SetSphereRadius(SafeZoneRadius);
	SafeZoneComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARW_ZonesActor::SafeZoneBeginOverlap);
	SafeZoneComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ARW_ZonesActor::SafeZoneEndOverlap);
	SafeZoneComponent->SetCollisionObjectType(ECC_GameTraceChannel12);
	SafeZoneComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SafeZoneComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ARW_ZonesActor::BeginPlay()
{
	Super::BeginPlay();
	
	const auto ScenarioData = GetWorld()->GetGameInstance<URW_GameInstance>()->ScenarioData;
	const auto ExperienceManager = Cast<ARW_ExpGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetExperienceManager();
	const auto ScenarioManager = Cast<URW_ScenarioManager>(ExperienceManager->GetManagerRef_Implementation(URW_ScenarioManager::StaticClass()));

	const EEquipmentGear NeededEquipmentGear = ScenarioManager->GetEquipmentGearByScenario(ScenarioData->HazardousMaterial, ScenarioData->IsFire);
	SetNeededEquipmentGear(NeededEquipmentGear);

	const auto ApproachDistance = GetWorld()->GetGameInstance<URW_GameInstance>()->ScenarioData->ApproachDistance;

	if(ApproachDistance != Distance)
	{
		Destroy(); return;
	}
	
	const FString MessageLog =  "Needed Equipment Gear: " + UEnum::GetDisplayValueAsText(NeededEquipmentGear).ToString();
	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Green, MessageLog);
}

void ARW_ZonesActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	DeathTimerTime -= DeltaSeconds;
	
	if (DeathTimerTime <= 0)
	{
		if (Character && !Character->IsPendingKillPending())
		{
			SetActorTickEnabled(false);
			EventLogService::NewEvent<UFinishScenarioEvent>(nullptr, EScoringMessageType::SMT_IncorrectEquipment);
		}
	}
}

void ARW_ZonesActor::SafeZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->ImplementsInterface(URWEquipmentInterface::StaticClass()))
	{
		IRWCoughingInterface::Execute_DisableCoughing(OtherActor);
		SetActorTickEnabled(false);
		Character = nullptr;
		DeathTimerTime = GetDefault<ARW_ZonesActor>()->DeathTimerTime;
	}
}

void ARW_ZonesActor::SafeZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->GetClass()->ImplementsInterface(URWEquipmentInterface::StaticClass()))
	{
		const EEquipmentGear& CurrentGear = IRWEquipmentInterface::Execute_GetEquipmentGear(OtherActor);
		if(CurrentGear != NeededGear && OtherActor->GetClass()->ImplementsInterface(URWEquipmentInterface::StaticClass()))
		{
			IRWCoughingInterface::Execute_EnableCoughing(OtherActor);
			Character = OtherActor;
			SetActorTickEnabled(true);
		}
		else
		{
			EventLogService::NewEvent<USetEquipmentEvent>(nullptr, CurrentGear);
		}
		
		EventLogService::NewEvent<ULeaveDangerZoneEvent>(nullptr);
	}
}
