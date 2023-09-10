# UnrealEngine4 useful C++ code.

A lot of  UE4 programming examples are done with BluePrints.
While such an approach is fine for small scripting logic,when it comes to complex scenarios the whole node structure becomes hardly readble and maintainable.Also programmers usually prefer 'real' code,instead of fancy visual coding framework. Here I release some of my own implementations in C++ which I use in personal projects and which I am really happy to share with others.

## Pawn based camera setup

SLPawn.h SLPawn.cpp

 Comes with 3 modes: Orbit,Trackball, Free (FPS) camera.
 
 TrackBall mode takes an object to pick,you will have to change name string of raycast query.
 It is not parametric at the moment.
 
 
 SLPLayerController can be replaced by default PlayerController.
 
 Tested on 4.21 4.22
 
 ## Customized virtual keyboard for Android with "Send" button at the same line with input text field.
 
 Modified version of GameActivity.java.template which adds a message send button like the on in Whatsapp messenger to
 UE Android Virtual Keyboard.  Unreal Engine 5.2
 
 
 ## Bug fixes and other contributions are welcomed.
 
 
 
