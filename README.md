# Unreal Engine 5 Interaction System:
 This is an Interaction System for Unreal Engine 5; It handles player interaction with any interactable world object.

### Pros:
* Pretty much plug and play; All logic is ran from a single actor component ```InteractionComponent``` and a interface ```InteractionInterface```
* Uses interfaces; So no need to reparent any of your existing actors to a base interactable class
* Easy to use; Straight-forward with no bloat
* Blueprint accessable; No code required
* Uses timers; Not tick
* Clean and commented

### Cons:
* Not network replicated
---

## Getting Started:
1. First thing you need to do is add the ```Interaction``` component to your player character/pawn  

![image](https://github.com/user-attachments/assets/4246b564-c29c-4442-a5b5-49e7e8b97a29)

2. Once the component has been added; tweak the settings to your liking  

![image](https://github.com/user-attachments/assets/3f5b766a-65ab-41ed-b695-7ec18b932152)

3. Add the ```InteractionInterface``` to any of the actors you wish to be seen as an ```Interactable```  

![image](https://github.com/user-attachments/assets/b1926c37-70fd-4921-9365-44307b86c382)

and thats it! You're now all set and can start using the interaction plugin!

---

## Using the Plugin:
Using the plugin is very simple; all you need to do is call ```StartInteractionTrace()``` and it will start tracing for interactables (if you have ```bStartOnBeginPlay``` set as ```true``` you can skip this).  

If we hit an ```Interactable``` then it will update the ```TargetActor``` variable in the component

![image](https://github.com/user-attachments/assets/383ec7fb-089b-43b0-aeee-cc3fe867d457)

Stopping the trace is just as easy; Just call ```StopInteractionTrace()``` and the trace will forget the ```TargetActor``` and stop.

![image](https://github.com/user-attachments/assets/550434b8-3ec5-4367-a7eb-741dba02cd4a)

We can interact with our ```TargetActor``` by calling the ```InteractWithTargetActor()``` function.  This will simply check if the ```TargetActor``` is valid and has the ```InteractableInterface``` then interacts with it.

![image](https://github.com/user-attachments/assets/acd7d176-6608-463c-ba9b-e9e47600b182)

### Delegates:
There are a few ```Delegates``` that you can bind too when certian things happen from within the interaction system.  

![image](https://github.com/user-attachments/assets/9aa7731c-5e7f-479f-82ae-dd39dfd825fa)

#### OnTargetActorUpdated:
Fires when the trace hits a new actor and updates the ```TargetActor``` in the component  

 ![image](https://github.com/user-attachments/assets/999bbbb1-1a4c-4ece-9d76-4473691e016c)

 #### OnTargetActorForgotten:
 Fires when the ```TargetActor``` has been forgetten or replaced

![image](https://github.com/user-attachments/assets/08d2e9f7-8df7-40ab-9c49-57cf0337926f)

---

## Interaction Interface:
The ```InteractionInterface``` is how we can detect ```Interactables``` without having to convert any actors to a child of a base interactable class. Just add this interface to any actor you want to be deemed as an ```Interactable``` and the component will do the rest!

### Interface Functions:
The ```InteractionInterface``` has a few functions which you would need to override so you can select funtionality dependant on the actor it is overridden in.

![image](https://github.com/user-attachments/assets/50684449-85ff-41d4-af59-3181c643fc58)

#### Interact:
```Interact()``` is what gets called on the ```Interactable``` when ```InteractWithTargetActor()``` is called. This is the bare minimum needed for the ```Interactable``` to work.

![image](https://github.com/user-attachments/assets/a7d384f0-1a4b-4228-a38d-aed02db8a849)

#### BecomeTarget:
Not as important; but there is also ```BecomeTarget()```. Override this and you can add funtionality for when the ```Interactable``` becomes the ```TargetActor```

![image](https://github.com/user-attachments/assets/9fb63d01-e8bb-4968-a15b-c18b711b2dc8)

#### ForgottenAsTarget:
Not as important; but there is also ```ForgottenAsTarget()```. Override this and you can add funtionality for when the ```Interactable``` is no longer the ```TargetActor```

![image](https://github.com/user-attachments/assets/9194f5c7-d339-43f6-9e5f-550d60b5f72e)

---
## Tutorial Video

[![Watch the video](https://img.youtube.com/vi/C9gw4YmVxj0/maxresdefault.jpg)](https://www.youtube.com/watch?v=C9gw4YmVxj0)

---

## License
This plugin is licensed under **CC BY-NC 4.0**.  
You can use it for free in commercial and personal projects, but **you cannot sell or distribute** this plugin or any modified version of it.  
See the full license: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)

---
