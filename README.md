# UECollisionQueryTools
This repo contains code for the CollisionQueryTestActor tool, mentioned in the Unreal Fest 2023 talk _Making Sense Of Collision Data_. A recording of the talk is available here: https://www.youtube.com/watch?v=xIQI6nXFygA

This is a simple actor to help with testing collision queries and understanding collision data in levels in Unreal Engine. The actor can be configured to perform any collision query on tick.

Additionally several debug draw helper functions are included for drawing swept shapes, which the tool makes use of.

## Compatibility
The tool has been tested in Unreal Engine 5.1 and 5.2

## How do I use it?
1. Copy the code files into a suitable directory in your project. Compile the editor.
2. Place a CollisionQueryTestActor in your level using the Place Actors window.
   
![CollisionQueryTestActor in Place Actors window](/Images/image01.PNG) ![CollisionQueryTestActor in level](/Images/image02.PNG)

4. Start Play In Editor (PIE) or start Simulating.
    * The CollisionQueryTestActor should be drawing a red (or green) debug line.

![Start Play in Editor](/Images/image03.PNG)

5. Move and rotate the actor to define the start transform of the query.
   * You can also move the actor's EndComponent to separately define the end transform of the query.

![Line trace with no hit](/Images/image04.PNG) ![Line trace with successful hit](/Images/image05.PNG)

6. Configure the query via the Details Panel with the actor selected
    * Switch between line trace, sweep, and overlap test
    * Switch between tracing/sweeping by channel, profile, and object type
    * Switch between single/multi trace/sweep
    * Configure the shape of a sweep / overlap test
    * Configure the collision channel and responses
    * Configure other query parameters (eg. bTraceComplex)

![Actor properties in Details Panel](/Images/image07.PNG)

7. See the result of the query update live via the debug draw
    * Green = hit, Red = no hit, Blue = overlap
    * Traces / sweeps will be drawn to where they stopped
    * Points will be drawn to indicate hit / overlap locations 

![Capsule sweep debug draw](/Images/image08.PNG)
