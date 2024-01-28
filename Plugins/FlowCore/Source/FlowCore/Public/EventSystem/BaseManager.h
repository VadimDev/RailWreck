#pragma once

#include <unordered_map>
#include "CoreMinimal.h"
#include "EventLogService.h"
#include "UObject/Object.h"
#include "BaseManager.generated.h"

#define DECLARE_EVENT_HANDLER(TEvent) protected: void HandleEvent_Impl(class TEvent* Event);

/*
 The DEFINE_EVENT_HANDLER macro is designed to define an event handler.
 Its purpose is to create an entry in the Handlers dictionary, associating a specific event type with its corresponding handling method.

 1. static_assert:
	std::is_base_of<UBaseEvent, TEventType>::value is a compile-time check ensuring that the type TEventType is derived from UBaseEvent.
	In other words, this code checks that the provided event type is indeed an event.
	If TEventType isn't derived from UBaseEvent, the compilation will fail with the message "Trying to create definition for invalid event".

 2. Adding the handler to the dictionary:
	Handlers[TEventType::StaticClass()] represents an entry in the Handlers dictionary where the key is the static class of the event,
	and the value will be a lambda function defined next.

3. Lambda function:
	[this](UBaseEvent* Event) -> void defines a lambda function that takes one argument of type UBaseEvent* and returns void.
	The lambda function captures the current object (this), enabling it to call the object's methods.
	Inside the lambda function, there's a call to HandleEvent_Impl(Cast<TEventType>(Event)).
	Cast<TEventType>(Event) is a way of casting from UBaseEvent* to TEventType*.
	If the type cast is successful, the corresponding HandleEvent_Impl method is called to handle that event.

 In essence, this macro allows for the automatic registration of event handlers in the Handlers dictionary for various event types.
 Upon receiving an event, the system will search for the corresponding handler in the dictionary and invoke it.
 If no handler is found, the HandleEvent_Impl method is called, which in this code generates an error.
 */
#define DEFINE_EVENT_HANDLER(TEventType)																			\
static_assert(std::is_base_of<UBaseEvent, TEventType>::value, "Trying to create definiton for invalid event");		\
Handlers[TEventType::StaticClass()] = [this](UBaseEvent* Event) -> void												\
{																													\
HandleEvent_Impl(Cast<TEventType>(Event));																			\
};

class UBaseEvent;

/*
The class serves as the base for all managers in the project and includes macros for declaring event handlers.
Let's say we need to create a UserManager to receive events.
	1. We inherit from UProject_UserManager : public UBaseManager
	2. We use the macro DECLARE_EVENT_HANDLER(UStartPlayEvent) which is part of UBaseManager and inserts the following code at the location:
		#define DECLARE_EVENT_HANDLER(TEvent) protected: void HandleEvent_Impl(class TEvent* Event);
	3. In the constructor of UProject_UserManager(), we use the macro DEFINE_EVENT_HANDLER(UStartPlayEvent) from UBaseManager
		"Its purpose is to create an entry in the Handlers dictionary, associating a specific event type with its corresponding handling method"
	4. Then we declare the event handler method for UStartPlayEvent, which will be called as soon as the appropriate event arrives:
		void UTD_HubButtonsManager::HandleEvent_Impl(UStartPlayEvent* Event)"
*/
UCLASS()
class FLOWCORE_API UBaseManager : public UObject
{
	GENERATED_BODY()

public:
	template<typename TEventType, typename ...TInitializeArgs>
	void SendEvent(TInitializeArgs... Arguments)
	{
		EventLogService::NewEvent<TEventType>(Arguments...);
	}

	void HandleEvent(UBaseEvent* Event);
	
private:
	// THandlers map keys
	using THandlerKey = UClass*;
	// THandlers map values
	using THandler = TFunction<void(UBaseEvent*)>;

	// Custom hash function for THandlers map, use the pointer address as a hash, this will give different values for different pointers
	struct THandlerHash {
		std::size_t operator()(THandlerKey Code) const
		{
			return reinterpret_cast<std::size_t>(Code);
		}
	};

	// Custom comparison function for THandlers map
	struct THandlerComp {
		bool operator()(const THandlerKey LHS, const THandlerKey RHS) const
		{
			return LHS == RHS;
		}
	};
	
	using THandlers = std::unordered_map<THandlerKey, THandler, THandlerHash, THandlerComp>;
	
protected:
	THandlers Handlers;
};