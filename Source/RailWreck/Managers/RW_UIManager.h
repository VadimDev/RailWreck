#pragma once

#include "CoreMinimal.h"
#include "EventSystem/BaseManager.h"
#include <concepts>
#include "RW_UIManager.generated.h"

template <class T>
concept is_actor_derived = std::is_base_of_v<AActor, T>;

UCLASS(Blueprintable)
class RAILWRECK_API URW_UIManager : public UBaseManager
{
	GENERATED_BODY()
	
public:
	template <is_actor_derived T>
	T* GetUIActorByType() const
	{
		if (const TSoftObjectPtr<AActor>* FoundActor = UIActors.Find(T::StaticClass()); FoundActor && FoundActor->IsValid())
			return Cast<T>(FoundActor->Get());
		return nullptr;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Actors")
	TMap<UClass*, TSoftObjectPtr<AActor>> UIActors;
};