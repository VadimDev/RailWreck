#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PPETypes.generated.h"

class UStaticMesh;

UENUM()
enum class EPPESetupType : uint8 
{
	PST_None,
	PST_Instructor,
	PST_User
};

UENUM(Blueprintable)
enum class EPPEItemType : uint8
{
	PIT_None,
	PIT_Gloves,
	PIT_HardHat,
	PIT_Vest,
	PIT_Glasses
};

UENUM(Blueprintable)
enum class EPPEAssetType : uint8
{
	PAT_None,
	PAT_StaticMesh,
	PAT_SkeletalMesh
};

USTRUCT(BlueprintType)
struct FLOWCORE_API FPPESetup
{
	GENERATED_BODY()
	
	FPPESetup() = default;
	EPPEItemType GetType() const { return Type; }
	UObject* GetMesh() const { return Mesh; }
	EPPEAssetType GetAssetType() const { return AssetType; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) EPPEItemType Type;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UStreamableRenderAsset* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EPPEAssetType AssetType;
};

USTRUCT(BlueprintType)
struct FLOWCORE_API FPPEResources
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TArray<FPPESetup> MeshAssetsData;
};
