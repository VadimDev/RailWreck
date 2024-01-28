#include "RW_CarriageActor.h"
#include "Components/TextRenderComponent.h"

ARW_CarriageActor::ARW_CarriageActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* DefaultRootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
	SetRootComponent(DefaultRootComponent);

	CarriageMesh = CreateDefaultSubobject<UStaticMeshComponent>("CarriageMesh");
	CarriageMesh->SetupAttachment(GetRootComponent());

	if (!IsRunningDedicatedServer())
	{
		TrainID1 = CreateDefaultSubobject<UTextRenderComponent>("TrainID1");
		TrainID1->SetupAttachment(GetRootComponent());
		TrainID1->AddLocalRotation(FRotator(0.f, 90.f, 0.f));
		TrainID1->SetWorldSize(50.f);
		TrainID1->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
		TrainID1->SetText(FText::FromString(TrainIDText));

		TrainID2 = CreateDefaultSubobject<UTextRenderComponent>("TrainID2");
		TrainID2->SetupAttachment(GetRootComponent());
		TrainID2->AddLocalRotation(FRotator(0.f, -90.f, 0.f));
		TrainID2->SetWorldSize(50.f);
		TrainID2->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
		TrainID2->SetText(FText::FromString(TrainIDText));
	}
}

void ARW_CarriageActor::UpdateTrainIDText(const FString& InTrainIDText)
{
	TrainIDText = InTrainIDText;
	if (GetNetMode() != NM_DedicatedServer)
	{
		TrainID1->SetText(FText::FromString(TrainIDText));
		TrainID2->SetText(FText::FromString(TrainIDText));
	}
}