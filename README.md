<ins>*The project was created for a resume and is not compilable; only headers are provided for review.*</ins>

The project was created with the aim of training people in various specialties in a virtual reality environment.
The foundation of everything is transferred to the FlowCore plugin, which contains an Event-Managers system based on the observer pattern, which is tied into the game mode (from which we can get the ExperienceManager, and from it, for example, the ScenarioManager, UIManager, get the resource we need, or subscribe to the mailing list of events we are interested in, such as the use of tools, game events, expired game timers.

It also has basic classes for characters, controllers, game modes, and various objects (VR hands, laser for interaction with the world, teleport, wrist menu through which a submenu of hints, tools, and exit from training mode is called), interfaces that are used from project to project. I have provided detailed comments on the main points.

[BaseManager](https://github.com/VadimDev/RailWreck/blob/master/Plugins/FlowCore/Source/FlowCore/Public/EventSystem/BaseManager.h) <br />
[EventLogService, EventContainer](https://github.com/VadimDev/RailWreck/blob/master/Plugins/FlowCore/Source/FlowCore/Public/EventSystem/EventLogService.h) <br />
[ExperienceManager](https://github.com/VadimDev/RailWreck/blob/master/Plugins/FlowCore/Source/FlowCore/Public/EventSystem/ExperienceManager.h) <br />

Also, the VRExpansionPlugin is used: https://github.com/mordentral/VRExpansionPlugin

![alt text](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/Scheme.png)

![alt text](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/1.jpg)

**BaseManager**

The class serves as the base for all managers in the project and includes macros for declaring event handlers.
Let's say we need to create a UserManager to receive events.

1. We inherit from ```UProject_UserManager : public UBaseManager```
2. We use the macro ```DECLARE_EVENT_HANDLER(UStartPlayEvent)``` which is part of UBaseManager and inserts the following code at the location:<br />
```#define DECLARE_EVENT_HANDLER(TEvent) protected: void HandleEvent_Impl(class TEvent* Event);```
3. In the constructor of UProject_UserManager(), we use the macro ```DEFINE_EVENT_HANDLER(UStartPlayEvent)``` from UBaseManager <br />
   Its purpose is to create an entry in the Handlers dictionary, associating a specific event type with its corresponding handling method
4. Then we declare the event handler method for UStartPlayEvent, which will be called as soon as the appropriate event arrives: <br />
		```void UTD_HubButtonsManager::HandleEvent_Impl(UStartPlayEvent* Event)```

> void UBaseManager::HandleEvent(UBaseEvent* Event)

This method checks if there's a handler for the passed event, and if one exists, it invokes that handler. If no handler exists for the event, the method simply terminates without action.

The DEFINE_EVENT_HANDLER macro is designed to define an event handler. <br />
Its purpose is to create an entry in the Handlers dictionary, associating a specific event type with its corresponding handling method.

**UBaseEvent**

UBaseEvent represents a base event that can be extended to create specific event types. 
- The Executor property holds a reference to the player controller that initiated the event.
- The Guid property is a unique identifier for the event.
- ListenersNum is an atomic integer that keeps track of the number of listeners currently handling the event.
- The class provides methods for initialization, locking, unlocking, and comparison of events.

**EventContainer**

This system allows for the creation, storage, and processing of events in an efficient and controlled manner, ensuring the safety and integrity of event data in a multi-threaded environment through the use of a double buffer and event lock/unlock methods.

EventContainer use Singleton design pattern for event management within a system and designed to store events of type UBaseEvent. The events are represented as two buffers (arrays), which can be efficiently swapped for adding and processing events.

**ExperienceManager**

Base class for the central manager on the map, which will contain references to all other game sub-managers. Derived from it, it is set in APixoVRGameMode from which the game's GameMode will inherit: <br />
```TSubclassOf<UExperienceManager> ExperienceManagerClass;``` <br />
In AFlowVRGameMode, the manager is created, and the Tick is executed, calling TickExperience, which calls HandleEvents() <br />
```
APixoVRGameMode::InitGame
    ExperienceManager = NewObject<UExperienceManager>(this, ExperienceManagerClass)
    ExperienceManager->InitExperience()
			
APixoVRGameMode::Tick(float DeltaSeconds)
    ExperienceManager->TickExperience(DeltaSeconds)
        inside which the function HandleEvents() is called

UExperienceManager::HandleEvents()
    every tick gets EventLogService::Events().GetAll(OutEvents) (static)
    and sends events to subscribers DispatchEvent(Event)
		
UExperienceManager::DispatchEvent(UBaseEvent* Event) passes the event to each sub-manager
    for (UBaseManager* Listener : SingleListeners)
    {
        Listener->HandleEvent(Event);
    }
```
