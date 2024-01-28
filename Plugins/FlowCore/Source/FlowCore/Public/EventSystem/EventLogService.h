#pragma once

#include "Events/BaseEvent.h"

/*
 *  This system allows for the creation, storage, and processing of events in an efficient and controlled manner,
 *  ensuring the safety and integrity of event data in a multi-threaded environment through the use of a double buffer and event lock/unlock methods.
 */

/*
	EventContainer use Singleton design pattern for event management within a system and designed to store events of type UBaseEvent.
	The events are represented as two buffers (arrays), which can be efficiently swapped for adding and processing events.
*/
class EventContainer
{
	friend class EventLogService;
	
	EventContainer() {}
	
public:
	/* Prevent copy cloning, assigning */
	EventContainer(EventContainer& Other) = delete;
	void operator=(const EventContainer&) = delete;

	// A method to retrieve all events from the current buffer and subsequently clear the buffer. Returns true if there were events in the buffer.
	bool GetAll(TArray<UBaseEvent*>& OutEvents);
	FORCEINLINE int32 Size() { return GetBuffer().Num(); };

	// A method to add a new event to the current buffer. Before adding, it checks the event’s validity and “locks” it to prevent modifications elsewhere in the system.
	FORCEINLINE bool Add(UBaseEvent* NewEvent)
	{
		if (NewEvent && NewEvent->Lock())
		{
			GetBuffer().Add(NewEvent);
			return true;
		}
		return false;
	}
	
private:
	FORCEINLINE TArray<UBaseEvent*>& GetBuffer()
	{
		return FirstBufferInUse ? FirstBuffer : SecondBuffer;
	}

	// A method to switch between the buffers and clear the current buffer.
	void Swap();
	
	TArray<UBaseEvent*> FirstBuffer;
	TArray<UBaseEvent*> SecondBuffer;
	
	bool FirstBufferInUse = true;
};

/*
 *  The EventLogService class is used for creating and managing events.
 *  It also implements the Singleton pattern, providing a static method to access the single instance of EventContainer.
 */
class FLOWCORE_API EventLogService
{
	friend class UExperienceManager;

public:
	/* Prevent copy cloning, assigning */
	EventLogService(EventLogService& Other) = delete;
	void operator=(const EventLogService&) = delete;

	/*
	After the event is created, it is added to the EventContainer. If the addition is successful, the event is initialized with the provided arguments.
	The method returns a pointer to the created event.
	*/
	template<typename TEventType, typename ...TInitializeArgs>
	static TEventType* NewEvent(APlayerController* Executor = nullptr, TInitializeArgs... Arguments)
	{
		static_assert(std::is_base_of_v<UBaseEvent, TEventType>, "TEventType should derive from UBaseEvent");

		TEventType* Event = NewObject<TEventType>();
		if (Events().Add(Event))
		{
			Event->Initialize(Executor, Arguments...);
		}
		return Event;
	}
	
private:
	static EventContainer& Events();
};