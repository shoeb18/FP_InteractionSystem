# FP_InteractionSystem
First Person Interaction System in UE5.6


# System Design
Component Based Design
This system is built using a component-based and interface driven architecture

The core logic is implemented inside a custom component:
-- UPlayerInteractionComponent
Attached to the Any character and Bind Input Event inside player or where you are binding all inputs.
Assign Interactable widget and highlight material properties of UPlayerInteractionComponent.
And you are good to go...

This approach ensures : Reusability, Separation of concerns, Easy integration into any character

# Interface-Based Design
The system uses the IInteractable interface to define interaction behavior
Any actor that implements this interface can be interacted with, without requiring specific class dependencies

Interface Responsibilities:
Interact() → Defines what happens on interaction
GetInteractionType() → Returns Press or Hold
GetHoldDuration() → Duration for hold interactions
GetWidgetSpawnLocation() → Position for Interaction UI display

Using this interface ensures :
Loose coupling
High flexibility
Easy extensibility

# Cpp and Blueprints

# Where I used Cpp
Core gameplay systems were implemented in C++ like 
UPlayerInteractionSystem inside that
Overlap detection
Interaction handling
Input processing (press/hold logic)
These run frequently and benefit from
Faster execution, Lower overhead compared to Blueprint
Manages interaction state
Maintains interactable lists
Controls flow of interaction
Better structure for complex logic
Easier debugging and maintainability

# Where I used Bluprints
Blueprints I used for Interaction UI, Interactable Base Actor
The interaction widget is created in Blueprint
Layout
Text
Progress bar visuals
For control for designer to change accordingly how they want.

To Extend the system is pretty easy you have to add another type inside enum and additional interface functions. And implement its functionality. You don't need to modify existing code for that.

# future improvements
1.
Current limitation:
The system selects the first actor in InteractablesInRange

Improvement:
Use distance-based prioritization (closest object)
Or implement a camera-based line trace
Prevents incorrect selection when multiple objects overlap
Matches player intention more accurately

2.
I would like to add more interation types in this like SPAM Interact. you have to spam a interact
button for a while to interact. 

