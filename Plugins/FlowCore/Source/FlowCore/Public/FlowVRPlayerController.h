#pragma once

#include "CoreMinimal.h"
#include "VRPlayerController.h"
#include "Components/TimelineComponent.h"
#include "FlowVRPlayerController.generated.h"

UCLASS()
class FLOWCORE_API AFlowVRPlayerController : public AVRPlayerController
{
	GENERATED_BODY()

public:
	AFlowVRPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* Fade */

	UFUNCTION(Client, Reliable)
	void ClientFadeIn(float FadeDuration);

	UFUNCTION(BlueprintCallable, Category = "Fade")
	void FadeIn(float FadeDuration = 1.0f);

	UFUNCTION(Client, Reliable)
	void ClientFadeOut(float FadeDuration);

	UFUNCTION(BlueprintCallable, Category = "Fade")
	void FadeOut(float FadeDuration = 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fade")
	UCurveFloat* ChangingFadeOpacityCurve;

private:
	UFUNCTION()
	void FadeOpacityTimelineProgress(float Value);

	void ImmediateFadeIn();
	void ImmediateFadeOut();

	FTimeline FadeTimeline;
};
