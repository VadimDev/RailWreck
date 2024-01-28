#include "RW_InteractionButton.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "RailWreck/Core/RW_HubGameMode.h"
#include "RailWreck/Managers/RW_HubButtonsManager.h"
#include "RailWreck/Managers/Base/RW_HubExperienceManager.h"

ARW_InteractionButton::ARW_InteractionButton()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* DefaultRootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
	SetRootComponent(DefaultRootComponent);

	PedestalComp = CreateDefaultSubobject<UStaticMeshComponent>("Pedestal");
	PedestalComp->SetupAttachment(GetRootComponent());

	ButtonSupportComp = CreateDefaultSubobject<UStaticMeshComponent>("ButtonSupport");
	ButtonSupportComp->SetupAttachment(PedestalComp);
	ButtonSupportComp->SetRelativeLocation(FVector(0, 0, 107));
	ButtonSupportComp->SetRelativeRotation(FRotator(0, 0, 90));

	ButtonMovingComp = CreateDefaultSubobject<UStaticMeshComponent>("ButtonMoving");
	ButtonMovingComp->SetupAttachment(ButtonSupportComp);
	ButtonMovingComp->SetRelativeLocation(FVector(0, -1.4, 0));

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollisionComp->SetupAttachment(ButtonMovingComp);
	BoxCollisionComp->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
	BoxCollisionComp->SetBoxExtent(FVector(4, 1, 4));
	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARW_InteractionButton::OnBeginOverlapButton);

	PointLightComp = CreateDefaultSubobject<UPointLightComponent>("PointLight");
	PointLightComp->SetupAttachment(ButtonMovingComp);
	PointLightComp->SetIntensity(100.f);
	PointLightComp->SetAttenuationRadius(20.f);
	PointLightComp->SetSoftSourceRadius(10.f);
	PointLightComp->SetCastShadows(false);
	PointLightComp->SetLightColor(FLinearColor::Green);
	PointLightComp->SetRelativeLocation(FVector(0, -0.5, 0));

	TextRenderComp = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	TextRenderComp->SetupAttachment(PedestalComp);
	TextRenderComp->SetWorldSize(20.f);
	TextRenderComp->SetHorizontalAlignment(EHTA_Center);
	TextRenderComp->SetVerticalAlignment(EVRTA_TextCenter);
	TextRenderComp->SetRelativeLocation(FVector(0, 0, 150));
}

void ARW_InteractionButton::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MovingButtonTimeLine.TickTimeline(DeltaSeconds);
}

void ARW_InteractionButton::BeginPlay()
{
	Super::BeginPlay();

	if(HubButtonsName)
	{
		const FText* ButtonName = HubButtonsName->Names.Find(ButtonType);

		switch (ButtonType) {
		case EButtonType::SelectScenario:
			TextRenderComp->SetText(*ButtonName);
			break;
		case EButtonType::RandomizeScenario:
			TextRenderComp->SetText(*ButtonName);
			break;
		case EButtonType::ResetScenario:
			TextRenderComp->SetText(*ButtonName);
			break;
		case EButtonType::EnterScenario:
			TextRenderComp->SetText(*ButtonName);
			break;
		default: ;
		}
	}
	
	if (MovingButtonCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("MovingButtonTimelineProgress"));
		MovingButtonTimeLine.AddInterpFloat(MovingButtonCurve, TimelineProgress);
		
		FOnTimelineEventStatic TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("MovingButtonTimelineFinished"));
		MovingButtonTimeLine.SetTimelineFinishedFunc(TimelineFinished);
		
		MovingButtonTimeLine.SetLooping(false);
		MovingButtonTimeLine.SetDirectionPropertyName("TimelineDirection");
	}
}

void ARW_InteractionButton::OnBeginOverlapButton(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
												UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsPressed || GetButtonState() == EButtonState::BS_Disabled) return;
	
	ButtonPress();
}

void ARW_InteractionButton::ButtonPress_Implementation()
{
	bIsPressed = true;
	SetButtonState(EButtonState::BS_Pressed);
	TimelineDirection = ETimelineDirection::Forward;
	MovingButtonTimeLine.PlayFromStart();
	
	UGameplayStatics::PlaySoundAtLocation(
		this,
		ButtonPressSound, ButtonMovingComp->GetComponentLocation(),
		ButtonPressSound->GetVolumeMultiplier(),
		ButtonPressSound->GetPitchMultiplier(), 0.f,
		ButtonPressSound->AttenuationSettings
		);
	
	const auto HubGameMode = Cast<ARW_HubGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	const auto ExperienceManager = HubGameMode->GetExperienceManager();
	const auto HubButtonsManager = Cast<URW_HubButtonsManager>(ExperienceManager->GetManagerRef_Implementation(URW_HubButtonsManager::StaticClass()));
	HubButtonsManager->ButtonPress(this);
}

void ARW_InteractionButton::ButtonRelease_Implementation()
{
	ButtonPressedDelegate.Broadcast(PlayerController);
	TimelineDirection = ETimelineDirection::Backward;
	MovingButtonTimeLine.ReverseFromEnd();
}

void ARW_InteractionButton::SetButtonState(EButtonState InButtonState)
{
	CurrentButtonState = InButtonState;
	
	switch (CurrentButtonState)
	{
	case EButtonState::BS_Default:
		TextRenderComp->SetVisibility(true);
		PointLightComp->SetLightColor(FLinearColor::Green);
		ButtonMovingComp->SetVectorParameterValueOnMaterials("ButtonColor", FVector(0.207881f, 0.71875f, 0.074695f));
		break;

	case EButtonState::BS_Pressed:
		PointLightComp->SetLightColor(FLinearColor::Red);
		ButtonMovingComp->SetVectorParameterValueOnMaterials("ButtonColor", FVector(1.f, 0.f, 0.f));
		break;

	case EButtonState::BS_Disabled:
		TextRenderComp->SetVisibility(false);
		PointLightComp->SetLightColor(FLinearColor(0.1f,0.1f,0.1f));
		ButtonMovingComp->SetVectorParameterValueOnMaterials("ButtonColor", FVector(0.1f, 0.1f, 0.1f));
		break;

	default:
		break;
	}
}

EButtonType ARW_InteractionButton::GetButtonType()
{
	return ButtonType;
}

void ARW_InteractionButton::MovingButtonTimelineProgress(float Value) const
{
	ButtonMovingComp->SetRelativeLocation(FVector(ButtonMovingComp->GetRelativeLocation().X, Value, ButtonMovingComp->GetRelativeLocation().Z));
}

void ARW_InteractionButton::MovingButtonTimelineFinished()
{
	switch (TimelineDirection)
	{
	case ETimelineDirection::Forward:
		ButtonRelease();
		break;
	
	case ETimelineDirection::Backward:
		if(CurrentButtonState != EButtonState::BS_Disabled)
		{
			SetButtonState(EButtonState::BS_Default);
		}
		ButtonReleasedDelegate.Broadcast(PlayerController);
		bIsPressed = false;
		break;
	
	default:
		break;
	}
}