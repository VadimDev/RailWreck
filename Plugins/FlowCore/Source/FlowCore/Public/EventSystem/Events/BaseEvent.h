#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseEvent.generated.h"

/*
	UBaseEvent represents a base event that can be extended to create specific event types.
	The Executor property holds a reference to the player controller that initiated the event.
	The Guid property is a unique identifier for the event.
	ListenersNum is an atomic integer that keeps track of the number of listeners currently handling the event.
	The class provides methods for initialization, locking, unlocking, and comparison of events.
*/
UCLASS(Abstract)
class FLOWCORE_API UBaseEvent : public UObject
{
	GENERATED_BODY()
	
public:
	// Initializes Executor to nullptr and assigns a new unique GUID to Guid.
	UBaseEvent();
	// SetExecutor
	void Initialize(APlayerController* NewExecutor);
	// GetExecutor
	APlayerController* GetExecutor() const;
	const FGuid& GetGuid() const { return Guid; }
	/*
	 Lock Method: increments ListenersNum.
	 If ListenersNum was 0 before incrementing, it adds the event to the root set to prevent it from being garbage collected.
	 Returns true if the event is valid, false otherwise, and logs a warning if trying to acquire an invalid event.
	 */
	virtual bool Lock();
	/*
	 Unlock Method: Decrements ListenersNum.
	 If ListenersNum becomes 0, it removes the event from the root set.
	 If the event becomes invalid while it's still being acquired, it logs an error.
	 */
	virtual void Unlock();
	
private:
	UPROPERTY() APlayerController* Executor;
	FGuid Guid;
	
	std::atomic<int32> ListenersNum;
};