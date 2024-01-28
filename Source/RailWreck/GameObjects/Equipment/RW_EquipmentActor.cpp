#include "RW_EquipmentActor.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RailWreck/Core/RW_PlayerCharacter.h"

ARW_EquipmentActor::ARW_EquipmentActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>("EquipmentMesh");
	EquipmentMesh->SetCollisionProfileName("BlockAllDynamic");
	SetRootComponent(EquipmentMesh);

	EquipmentName = CreateDefaultSubobject<UTextRenderComponent>("EquipmentName");
	EquipmentName->SetupAttachment(GetRootComponent());
	EquipmentName->SetHiddenInGame(true);
	EquipmentName->SetWorldSize(14.f);
	EquipmentName->SetHorizontalAlignment(EHTA_Center);
	EquipmentName->SetVerticalAlignment(EVRTA_TextCenter);

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(GetRootComponent());
	SphereCollision->SetCollisionProfileName("BlockAllDynamic");
	SphereCollision->SetCollisionObjectType(ECC_WorldStatic);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ARW_EquipmentActor::OnBeginOverlap);
}

void ARW_EquipmentActor::BeginPlay()
{
	Super::BeginPlay();

	DefaultPosition = GetActorTransform();
}

void ARW_EquipmentActor::Tick(float DeltaSeconds)
{
	if(MotionController)
	{
		static float SquaredDistanceToReset = FMath::Pow(DistanceToReset, 2);
		const float SquaredDistance = (GetActorLocation() - DefaultPosition.GetLocation()).SizeSquared2D();
		if(SquaredDistance > SquaredDistanceToReset)
		{
			ForceDropObjectAndResetPosition(true);
		}
	}
	
	if(bIsDropped)
	{
		TimeToReset -= DeltaSeconds;
		if(TimeToReset <= 0.f)
		{
			ForceDropObjectAndResetPosition(true);
		}
	}
}

void ARW_EquipmentActor::SetDenyGripping(bool bDenyGripping)
{
	VRGripInterfaceSettings.bDenyGripping = bDenyGripping;
}

bool ARW_EquipmentActor::DenyGripping_Implementation(UGripMotionControllerComponent * GripInitiator)
{
	return VRGripInterfaceSettings.bDenyGripping;
}

void ARW_EquipmentActor::SetGripPriority(int NewGripPriority)
{
	VRGripInterfaceSettings.AdvancedGripSettings.GripPriority = NewGripPriority;
}

bool ARW_EquipmentActor::SimulateOnDrop_Implementation()
{
	return VRGripInterfaceSettings.bSimulateOnDrop;
}

void ARW_EquipmentActor::GetGripStiffnessAndDamping_Implementation(float &GripStiffnessOut, float &GripDampingOut)
{
	GripStiffnessOut = VRGripInterfaceSettings.ConstraintStiffness;
	GripDampingOut = VRGripInterfaceSettings.ConstraintDamping;
}

void ARW_EquipmentActor::OnGrip_Implementation(UGripMotionControllerComponent* GrippingController,
	const FBPActorGripInformation& GripInformation)
{
	SetOwner(GrippingController->GetOwner());
	MotionController = GrippingController;

	ResetTimer();
}

void ARW_EquipmentActor::OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController,
	const FBPActorGripInformation& GripInformation, bool bWasSocketed)
{
	LaunchTimer();
}

void ARW_EquipmentActor::OnHoverBegin_Implementation(const FHitResult& HitResult, APlayerController* Executor)
{
	if (!EquipmentName) { return; }
	EquipmentName->SetHiddenInGame(false);
}

void ARW_EquipmentActor::OnHoverEnd_Implementation(const FHitResult& HitResult, APlayerController* Executor)
{
	if (!EquipmentName) { return; }
	EquipmentName->SetHiddenInGame(true);
}

void ARW_EquipmentActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(MotionController && Cast<ARW_PlayerCharacter>(OtherActor) && OtherActor->GetRootComponent() == OtherComp)
	{
		if(OtherActor->GetClass()->ImplementsInterface(URWEquipmentInterface::StaticClass()))
		{
			if(EquipmentGear != EEquipmentGear::EG_None && IRWEquipmentInterface::Execute_GetEquipmentGear(OtherActor) != EquipmentGear)
			{
				IRWEquipmentInterface::Execute_SetEquipmentGear(OtherActor, EquipmentGear);
			}
			else if(EquipmentItem != EEquipmentItem::EI_None && IRWEquipmentInterface::Execute_GetEquipmentItem(OtherActor) != EquipmentItem)
			{
				IRWEquipmentInterface::Execute_SetEquipmentItem(OtherActor, EquipmentItem);
			}

			PlayEquipmentSound();
			ForceDropObjectAndResetPosition(false);
		}
	}
}

void ARW_EquipmentActor::PlayEquipmentSound() const
{
	if(EquipmentSound)
	{
		UGameplayStatics::PlaySound2D(this, EquipmentSound);
	}
}

void ARW_EquipmentActor::LaunchTimer()
{
	EquipmentMesh->SetSimulatePhysics(true);
	bIsDropped = true;
}

void ARW_EquipmentActor::ResetTimer()
{
	TimeToReset = GetDefault<ARW_EquipmentActor>()->TimeToReset;
	EquipmentMesh->SetSimulatePhysics(false);
	bIsDropped = false;
}

void ARW_EquipmentActor::ForceDropObjectAndResetPosition(bool SimpleDrop)
{
	if(!SimpleDrop)
	{
		if(MotionController)
		{
			FBPActorGripInformation GripInfo;
			EBPVRResultSwitch Result;
			MotionController->GetGripByObject(GripInfo, this, Result);

			if(Result == EBPVRResultSwitch::OnSucceeded)
			{
				MotionController->DropGrip(GripInfo);
				ResetTimer();
				SetActorTransform(DefaultPosition);
		
				MotionController = nullptr;
				SetOwner(nullptr);
			}
		}
	}
	else
	{
		ResetTimer();
		SetActorTransform(DefaultPosition);
		MotionController = nullptr;
		SetOwner(nullptr);
	}
}

