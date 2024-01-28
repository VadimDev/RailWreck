#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FlowVRWristMenuDataTableRow.generated.h"

UENUM(BlueprintType)
enum class EWristMenuItemCategory : uint8
{
	EXECUTETABLE UMETA(DisplayName="Executeable"),
	SUBMENU      UMETA(Displayname="Submenu"),
	UNKNOWN      UMETA(Displayname="Unknown")
};

UENUM(BlueprintType)
enum class EButtonSize : uint8
{
	NORMAL	UMETA(DisplayName="Normal"),
	LARGE	UMETA(DisplayName="Large")
};

USTRUCT(BlueprintType)
struct FLOWCORE_API FFlowVRWristMenuDataTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/// Text Title for this menu item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TextTitle;

	/// A Description of the menu item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	/// A texture that will be assigned to this menu item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconTexture;

	/// The categroy of the menu item. Can be a submenu or an executable.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWristMenuItemCategory MenuCategory;

	/// The menu item id.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MenuItemID;

	//The size of the menu button
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EButtonSize ButtonSize;

	//Button HighlightColor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor HighlightColor = FLinearColor(0.996f, 0.51f, 0.0f, 1.0f);
};