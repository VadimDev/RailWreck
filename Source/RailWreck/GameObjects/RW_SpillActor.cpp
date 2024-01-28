#include "RW_SpillActor.h"
#include "Components/HighlightComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RailWreck/Core/RW_ExpGameMode.h"
#include "RailWreck/Core/RW_GameInstance.h"
#include "RailWreck/Managers/RW_ScenarioManager.h"
#include "RailWreck/Managers/Base/RW_ExperienceManager.h"
#include "RailWreck/Managers/Events/OutletValveCloseEvent.h"
#include "RailWreck/Managers/Events/SpillEliminatedEvent.h"
#include "..\Managers\Events\UsedSlimeBombEvent.h"

ARW_SpillActor::ARW_SpillActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* DefaultRootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
	SetRootComponent(DefaultRootComponent);
	
	SpillMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SpillMesh->SetupAttachment(GetRootComponent());
	
	SpillWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	SpillWidgetComponent->SetupAttachment(GetRootComponent());
	SpillWidgetComponent->SetWorldScale3D(FVector(0.0, 0.0, 0.0));
	SpillWidgetComponent->SetVisibility(false);
	
	HighlightComponent = CreateDefaultSubobject<UHighlightComponent>("HighlightComponent");
}

void ARW_SpillActor::BeginPlay()
{
	Super::BeginPlay();

	const auto ExpGameMode = Cast<ARW_ExpGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	const auto ExperienceManager = ExpGameMode->GetExperienceManager();
	ScenarioManager = Cast<URW_ScenarioManager>(ExperienceManager->GetManagerRef_Implementation(URW_ScenarioManager::StaticClass()));
	
	if(!ScenarioManager->GameInstance->ScenarioData->IsSpill)
	{
		ScenarioWithoutSpillEvent();
		bIsEliminated = true;
	}
}

void ARW_SpillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
		SpillWidgetComponent->GetComponentLocation(),
		GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	TargetRotation.Roll = 0;
	TargetRotation.Pitch = 0;

	SpillWidgetComponent->SetWorldRotation(TargetRotation);
}

void ARW_SpillActor::FixTheSpill(ESpillWidgetType WidgetType)
{
	bIsEliminated = true;
	if(EliminationSound)
	{
		UGameplayStatics::PlaySound2D(this, EliminationSound);
	}

	HighlightComponent->UpdateHighlightValue(EHighlightStencilEnum::HSE_None);

	FixTheSpillEvent();

	switch (WidgetType)
	{
	case ESpillWidgetType::SWT_OutletValve:
		EventLogService::NewEvent<UOutletValveCloseEvent>();
		break;
	case ESpillWidgetType::SWT_SlimeBomb:
		EventLogService::NewEvent<UUsedSlimeBombEvent>();
		break;
	default:
		EventLogService::NewEvent<USpillEliminatedEvent>();
	}
}

void ARW_SpillActor::OnHoverBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor)
{
	HighlightComponent->UpdateHighlightValue(EHighlightStencilEnum::HSE_Incorrect);
}

void ARW_SpillActor::OnHoverEnd_Implementation(const FHitResult& HitResult, APlayerController* Executor)
{
	HighlightComponent->UpdateHighlightValue(EHighlightStencilEnum::HSE_None);
}

void ARW_SpillActor::OnPressBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor)
{
	if(!IsActorUsed)
	{
		OnActorPressEvent();
		IsActorUsed = true;
	}
}

bool ARW_SpillActor::DisableHoverEvents_Implementation()
{
	return bIsEliminated;
}

bool ARW_SpillActor::DisablePressEvents_Implementation()
{
	return bIsEliminated;
}