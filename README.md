<ins>*The project was created for a resume and is not compilable; only headers are provided for review.*</ins> <br />
The focus was on functionality, not on graphics, and Unreal Engine 5 was used.

The project was created with the aim of training people in various specialties in a virtual reality environment.
The foundation of everything is transferred to the FlowCore plugin, which contains an Event-Managers system based on the observer pattern, which is tied into the game mode (from which we can get the ExperienceManager, and from it, for example, the ScenarioManager, UIManager, get the resource we need, or subscribe to the mailing list of events we are interested in, such as the use of tools, game events, expired game timers.

It also has basic classes for characters, controllers, game modes, and various objects (VR hands, laser for interaction with the world, teleport, wrist menu through which a submenu of hints, tools, and exit from training mode is called), interfaces that are used from project to project. I have provided detailed comments on the main points.

[BaseManager](https://github.com/VadimDev/RailWreck/blob/master/Plugins/FlowCore/Source/FlowCore/Public/EventSystem/BaseManager.h) <br />
[EventLogService, EventContainer](https://github.com/VadimDev/RailWreck/blob/master/Plugins/FlowCore/Source/FlowCore/Public/EventSystem/EventLogService.h) <br />
[ExperienceManager](https://github.com/VadimDev/RailWreck/blob/master/Plugins/FlowCore/Source/FlowCore/Public/EventSystem/ExperienceManager.h) <br />

Also, the VRExpansionPlugin is used: https://github.com/mordentral/VRExpansionPlugin

![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/Scheme.png)

![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/1.jpg)

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

# Rail Wreck
The main task of the project is to create a simulation of a railway accident for practicing the containment of spills and fire involving hazardous materials, for training specialists, but in the safe environment of virtual reality.

Click on the link below to see the full demo: <br />
https://www.youtube.com/watch?v=PaD4DQmxG-A 

[<img src="https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/10.webp" width="50%">](https://www.youtube.com/watch?v=PaD4DQmxG-A)

The game begins in the Hub, where the user selects the scenario of interest:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/2.jpg)

Hazardous materials that will be encountered:

* Crude Oil
* Chlorine
* Liquid Propane Gas
* Sulfuric Acid

You can choose day or night time, the presence of fire on the map, and whether there is a chemical spill.

For interacting with the surrounding world, the player has several tools, including a **laser** allowing the selection of menu items and triggering actions on game objects, and a **teleport** for quick movement in the virtual environment.
After selecting a scenario, it is necessary to interact with the player's hands by pressing on columns simulating buttons.

From the Approach Distance widget, the gameplay begins, where you are prompted to choose a safe distance for positioning the rescue vehicle, and this result contributes to the final score:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/3.jpg)

If the distance is chosen incorrectly, a notification is displayed at the start:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/4.jpg)

Scene in the editor:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/5.jpg)
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/6.jpg)
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/7.jpg)
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/8.jpg)

Putting on and adding items is done by taking them in hands and bringing them close to the player's torso:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/9.webp)

All the inventory is located in the vehicle, and there are three types of fire hoses:

* Water
* Ground Monitor
* Foam

Some substances cannot be extinguished with water as it may cause an explosion:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/10.webp)

For the material chlorine, there are three different sets depending on the type of container randomly selected. These sets are located in boxes near the vehicle and are called Emergency Kit A, B, C:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/11.webp)

To eliminate a chemical spill, you need to click on the spill object with the laser. If you have the correct tool, you will be able to eliminate the spill, otherwise, a message will appear indicating that something is missing:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/12.webp)


To eliminate spills in tanks, there is a tool called the **Slime Bomb** which plugs the hole. When it is taken, it is added as a new object in the **Wrist Menu**. This is a convenient tool from which different sub-menus with inventory or game commands and hints in virtual reality can be called. It attaches to the hand: <br />
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/13.webp)
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/14.webp)

If we forget which substance we are dealing with, we can use the **material detector** by calling it from the Wrist Menu:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/15.webp)

For spills, it is necessary to set up a tent:
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/16.webp)


After completing all actions, or in case of critical errors, the user returns to the hub where they can review the results and start the training again: <br />
![](https://github.com/VadimDev/RailWreck/blob/master/ForReadme/webp/17.webp)
<br />
<br />
<br />
View of the hub and the structure of the project in the editor:

<img src="https://github.com/VadimDev/RailWreck/blob/master/ForReadme/18.jpg" width="70%"><img src="https://github.com/VadimDev/RailWreck/blob/master/ForReadme/19.jpg" width="70%">
