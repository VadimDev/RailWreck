#include "RW_TentInBag.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "EventSystem/EventLogService.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RailWreck/Core/RW_GameInstance.h"
#include "RailWreck/Core/RW_PlayerCharacter.h"
#include "RailWreck/Managers/Events/TentConfirmEvent.h"
#include "RailWreck/UI/RW_SpillWidget.h"
#include "UI/ButtonWidget.h"

class URW_GameInstance;

ARW_TentInBag::ARW_TentInBag()
{
	PrimaryActorTick.bCanEverTick = true;
	
	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>("EquipmentMesh");
	SetRootComponent(EquipmentMesh);
	EquipmentMesh->SetCollisionProfileName("BlockAllDynamic");

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(GetRootComponent());
	SphereCollision->SetCollisionObjectType(ECC_GameTraceChannel12);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TentWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	TentWidgetComponent->SetupAttachment(GetRootComponent());
	TentWidgetComponent->SetWorldScale3D(FVector(0.0, 0.0, 0.0));
	
	TextRender = CreateDefaultSubobject<UTextRenderComponent>("EquipmentName");
	if (TextRender)
	{
		TextRender->SetupAttachment(GetRootComponent());
		TextRender->SetHiddenInGame(true);
		TextRender->SetWorldSize(14.f);
		TextRender->SetHorizontalAlignment(EHTA_Center);
		TextRender->SetVerticalAlignment(EVRTA_TextCenter);
	}
}

void ARW_TentInBag::BeginPlay()
{
	Super::BeginPlay();

	const auto GameInstance = GetWorld()->GetGameInstance<URW_GameInstance>();
	if(!GameInstance->ScenarioData->IsSpill)
	{
		Destroy(); return;
	}
	
	const auto TentWidget = Cast<URW_SpillWidget>(TentWidgetComponent->GetWidget());
	
	TentWidget->ConfirmButton->OnClickEventDelegate.AddUniqueDynamic(this, &ARW_TentInBag::OnConfirmButtonClicked);
	TentWidget->CloseButton->OnClickEventDelegate.AddUniqueDynamic(this, &ARW_TentInBag::OnCloseButtonClicked);
}

void ARW_TentInBag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto PlayerCharacter = Cast<ARW_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	const FVector HeadLocation = PlayerCharacter->Head->GetComponentTransform().GetLocation();
	const FVector TextLocation = TextRender->GetComponentTransform().GetLocation();

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(HeadLocation, TextLocation);
	TargetRotation.Add(0, 180, 0);
	TargetRotation.Pitch *= -1;
	TextRender->SetWorldRotation(TargetRotation);
	
	TargetRotation.Roll = 0;
	TargetRotation.Pitch = 0;
	
	TentWidgetComponent->SetWorldRotation(TargetRotation);
}

void ARW_TentInBag::OnHoverBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor)
{
	TextRender->SetHiddenInGame(false);
}

void ARW_TentInBag::OnHoverEnd_Implementation(const FHitResult& HitResult, APlayerController* Executor)
{
	TextRender->SetHiddenInGame(true);
}

void ARW_TentInBag::OnCloseButtonClicked(FText ButtonName)
{
	OnCloseClickedEvent();
}

void ARW_TentInBag::OnConfirmButtonClicked(FText ButtonName)
{
	Destroy();
	EventLogService::NewEvent<UTentConfirmEvent>();
}