This page contains suggestions, code snippets and tips that are not elaborate enough to require their own page




---

# General #
## Python Snippets ##
### Creating a headlight ###
**Added:** ver. 3.3 - 29 January 2013
In some applications you want to have a light attached to the main camera: when the camera moves and rotates the light should follow. This is very easy to implement, given that the `Camera` class derives from `SceneNode`, and it can have children attached to it:
```
	# Create a full white headlight
	headlight = Light.create()
	headlight.setColor(Color("white"))
	headlight.setEnabled(True)
	getDefaultCamera().addChild(headlight)
```
Remember the light will be attached by default at the camera origin: inside a VR system this does not correspond to the head position of the tracked user. If you want to take the head offset into account, you can make the light follow the head trackable object. Add the following line:
```
	headlight.followTrackable(headTrackableId)
```
Where `headTrackableId` is the integer Id of the head trackable (check your VR system config file or tracker configuration for this).
Note that you need to attach the light to the camera **AND** to the head trackable to have a full headlight. Without attaching the light to the camera, the light will not move when navigating in the scene.

## FAQ ##
### "I set the camera position to (x, y, z) but the output is not what I expected" ###
**Added:** ver. 3.2 - 20 December 2012
Cameras in omegalib (and in other VR toolkits) work a little differently than what yo could expect in classic graphic engines. There are two things that influence the final camera view:
  * The camera position and orientation, which infuence the view transform. They determine the conversion between the physical space in your system , and the space used your virtual world.
  * The **head position and orientation**, which influence both the view and the projection transforms. They represent the position of the observer in the phisical space. head transform, camera transform and display geometry all contribute in determining what will appear on the screens.

### "In a VR system, how do I convert my trackable object position / orientation into world coordinates?" ###
**Added:** ver. 3.2 - 20 December 2012
Tracked objects like stereo glasses, wands etc. generate events with positions and orientations in **sensor space**. This space usually corresponds to the physical space used to describe the VR system structure. For instance in CAVE2 bot sensor space and VR system space cosider the origin as the center of the sphere (on the ground), with Z pointing forward.

When navigating within the scene, you move the camera to a different position and orientation. Consequently, tracked object positions do not make sense anymore, until you convert them to this new, "virtual world" space defined by the camera transformations. Luckily, the `Camera` class comes with method that do exactly this.

Given a `Vector3f trackerPosition`, you can convert it to the world reference frame doing (in C++)
```
	Camera* cam = Engine::instance()->getDefaultCamera(); // equivalent to the getDefaultCamera python call.
	cam->localToWorldPosition(Vector3f position);
	// use cam->localToWorldOrientation(orientation) to convert an orientation instead.
```

### "I'm trying to load an .obj file. The model does not show up, and the console gets spammed with a lot of CullVisitor Errors' ###
**Added:** ver. 3.7 - 29 April 2013
This is likely due to your obj model using **per-vertex colors**. The OpenSceneGraph obj loader does not like per-vertex colors. The typical solution is to use per-face colors instead. An easy way to do this is to open your mesh in a tool like MeshLab and use Filters > Color Creation and Processing > Vertex Colors to Face Colors
When saving the new obj **make sure you disable colors** in the vertex attributes section.

### "My model disappears when I get close / far from it, or it just shows up on a few tiles of my tiled display" ###
**Updated:** ver. 3.7 - 3 May 2013
This is likely a near/far clipping plane issue. It usually shows up when you are drawing big models (i.e. when using osgEarth).
Omegalib tries to set up good clipping planes for you but does not handle every situation well. You can manually specify the near and far clipping planes using the `setNearFarZ` function in python (or the equivalent `DisplaySystem` method in C++). A good test call is something like `setNearFarZ(0.1, 1000000)`. Note that if you have a big near-to-far z ratio, you may get z fighting issues for objects close in depth. Omegalib uses a 24-bit z buffer by default.
If your application is based on OpenSceneGraph (using cyclops for instance) you can also turn on **depth partitioning** to handle scenes with large Z intervals (i.e drawing planetary-scale data at a distance, plus small objects close to the camera).

For example, to enable depth partitioning and create two partitions, one for near objects (z from 0.1 to 1000) and one for far objects (z from 1000 to 100000000):
```
	# Specify the overall depth range
	setNearFarZ(0.1, 100000000)
	# Give a render hint to the OpenSceneGraph module (if loaded): create a depth partition at 1000. 
	queueCommand(`:depthpart on 1000`)
```



---

# Visual Studio 2010 #
omegalib supports builds in visual studio 2008 and 2010. Both 32 and 64 bit builds should work, but currently only
Visual studio 2010 32 bit builds are tested regularly.

## Troubleshooting ##
### "When building omegalib from scratch with equalizer enabled the build fails with the compiler complaining about not finding `lexer.cpp`" ###
This error is due to some incorrect dependencies in the equalizer project, and visual studio spawning multiple compiler processes on multiprocessor machines. Simply ignore the error and build the solution again. After one-two additional builds the problem should go away. This won't happen anymore, unless you rebuild omegalib from scratch.

### "When enabling DirectInput support, the build fails because `hidsdi.h` is not found" ###
For DirectInput to work, you have to make sure the Visual Studio compiler searches for DirectInput headers from the windows DDK.
<p align='middle'><img src='http://omegalib.googlecode.com/svn/wiki/DirectInputConfig.png' width='500' /></p><p align='middle'>
<i><sup>Make sure the WinDDK include directory is the first one in the include directories section under Project Options >> Projects and Solutions >> VC++ Directories</sup></i>
</p>

### "The build fails because the equalizer header 'api.h' is noy found" ###
Similar to the 'lexer.cpp' error. Sometimes building again all that is needed to solve the problem. Otherwise, to make sure it goes away, build equalizer separately (see the picture) the do the same for the omega project
<p align='middle'><img src='http://omegalib.googlecode.com/svn/wiki/HOWTO2.png' width='500' /></p><p align='middle'>
<i><sup>How to build equalizer separately to solve this error.</sup></i>
</p>

### "In Debug mode the build succeeds but the program will fail running with a 0xc0150002 error" ###
Some of the libraries used internally by omegalib need the Visual Studio 2008 Debug CRT to run.
To solve this issue install Visual C++ Express 2008 (post link)

### "The build fails because svn complains: the equalizer build directory is not a working copy" ###
The cause of this error is unclear, and so far it happened on only one virtual machine install. Equalizer and omegalib actually build fine with this error, but Visual Studio will keep complaining about it.

A workaround is to **exclude** the equalizer project from the build after building the entire project the fist time (see the **Speeding up builds** tip in the general tips & tricks section)

### Other reasons for build failure ###
Make sure that the path to your source and build directories **does not contain spaces**
For example:
`C:/Users/Me/omegalib/build` is fine
`C:/Users/Alessandro Febretti/omegalib/build` is not.


## General Tips & Tricks ##
### Speeding up builds ###
After the first build, it is possible to speed up successive builds by unloading projects in the 3rdparty folder. These dependencies need to be built only once per configuration, so they can be disabled after the first build. This way, the build system won't have to go through them to check if they are up to date.
<p align='middle'><img src='http://omegalib.googlecode.com/svn/wiki/UnloadProjects.png' width='300' /></p><p align='middle'>
<i><sup>To unload third party projects, right-click on the 3rdparty folder and select "Unload Projects in Solution Folder"</sup></i>