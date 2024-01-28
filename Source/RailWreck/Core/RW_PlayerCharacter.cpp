#include "RW_PlayerCharacter.h"
#include "FlowVRWristMenu.h"
#include "Kismet/GameplayStatics.h"

void ARW_PlayerCharacter::SetEquipmentGear_Implementation(EEquipmentGear NewEquipmentGear)
{
	EquipmentGear = NewEquipmentGear;
	
	if(Gloves.Contains(EquipmentGear))
	{
		UpdatePPE(Gloves[EquipmentGear]);
	}
}

EEquipmentGear ARW_PlayerCharacter::GetEquipmentGear_Implementation() const
{
	return EquipmentGear;
}

void ARW_PlayerCharacter::SetEquipmentItem_Implementation(EEquipmentItem NewEquipmentItem)
{
	EquipmentItem = NewEquipmentItem;

	if(ensure(Titles.Contains(EquipmentItem)))
	{
		WristMenuItem.Description = Titles[EquipmentItem];
		WristMenuItem.TextTitle = FText::FromString(Titles[EquipmentItem]);
		WristMenu->AddIMenuItem(WristMenuItem);
	}
}

EEquipmentItem ARW_PlayerCharacter::GetEquipmentItem_Implementation() const
{
	return EquipmentItem;
}

void ARW_PlayerCharacter::EnableCoughing_Implementation()
{
	if(!CoughTimerHandler.IsValid())
	{
		MakeCough();
		GetWorld()->GetTimerManager().SetTimer(CoughTimerHandler, this, &ARW_PlayerCharacter::MakeCough, (CoughSound->Duration + CoughDelayTime), true);
	}
}

void ARW_PlayerCharacter::DisableCoughing_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(CoughTimerHandler);
}

void ARW_PlayerCharacter::OnWristMenuItemSelected_Implementation(UObject* WristMenuObject,
	const FFlowVRWristMenuDataTableRow& MenuItem)
{
	WristMenuDispatcher.Broadcast(MenuItem.MenuItemID, this);
}

void ARW_PlayerCharacter::MakeCough() const
{
	if(CoughSound)
	{
		UGameplayStatics::PlaySound2D(this, CoughSound);
	}
}
