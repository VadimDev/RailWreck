#pragma once

#include "CoreMinimal.h"
#include "Equipment/RW_EquipmentActor.h"
#include "GameFramework/Actor.h"
#include "RW_ZonesActor.generated.h"

UCLASS()
class RAILWRECK_API ARW_ZonesActor : public AActor
{
	GENERATED_BODY()

public:	
	ARW_ZonesActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void SetNeededEquipmentGear(const EEquipmentGear& InEquipmentGear) { NeededGear = InEquipmentGear; }
	EEquipmentGear GetNeededEquipmentGear() const { return NeededGear; }

private:
	UFUNCTION()
	void SafeZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void SafeZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)	USphereComponent* SafeZoneComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) float SafeZoneRadius = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) float DeathZoneRadius = 1200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup) float DeathTimerTime = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Setup) EApproachDistance Distance;
	
private:
	EEquipmentGear NeededGear;
	FTimerHandle DeathZoneTimerHandle;
	UPROPERTY() AActor* Character;
};
