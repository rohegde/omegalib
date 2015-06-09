# [PROJECT DEVELOPMENT MOVED TO GITHUB](https://github.com/febret/omegalib) #

<p align='middle'>
<wiki:gadget url="http://www.ohloh.net/p/487998/widgets/project_factoids_stats.xml" width="400" height="260" border="0"/><img src='http://omegalib.googlecode.com/svn/wiki/intro/intro3.jpg' width='400' />
</p>

omegalib is a middleware designed to ease the development of applications on virtual reality and immersive systems. Its main features are:
  * Support for hybrid systems, presenting high definition 2D and 3D content on the same display surface
  * A C++ and Python API. Applications can be developed as standalone executables in C++ or as scripts in Python. Omegalib also supports mixed native/script applications with user-defined C++ modules that can be exposed to Python through a simple declarative interface.
  * Runtime application switching: applications can be reloaded or swapped without restarting the display system.
  * Display system scalability: omegalib runs on desktop machines, multi-GPU workstations driving tiled displays, and cluster systems like the 36-machine, 72-display CAVE2.
  * Web interface control and pixel streaming to HTML5 clients
  * Experimental SAGE integration
  * A customizable 2D / 3D widgets library
  * Support for a wide range of input peripherals (controllers, motion capture systems, touch surfaces, speech and brain interfaces), through the [Omicron toolkit](http://code.google.com/p/omicron-sdk/)
  * Extendable integration with third party higher level toolkits like vtk and Open Scene Graph.
  * Sound playback through a supercollider-based sound server. Omegalib takes care of synchronizing sound assets between the application and sound machines of a VR installation. The sound server is scalable (it runs on laptop stereo speakers or on the 22 channel CAVE2 audio system) and can be customized using the supercollider scripting language.
Altough omegalib is designed for VR systems, it can be used to develop applications on standard desktop systems, leveraging the power of multiple GPU units when available.
The omegalib source distribution also includes all required main dependencies and **builds out-of-the-box** on Windows, Linux (32 and 64 bit) and OSX 10.7 or higher.

<p align='middle'><img src='http://omegalib.googlecode.com/svn/wiki/intro/rail.jpg' width='400' /><img src='http://omegalib.googlecode.com/svn/wiki/intro/brain.jpg' width='400' /></p>

<p align='middle'>
omegalib supports <b>OpenSceneGraph</b> applications with little change. The interactive 3d menu system can be controlled by mouse, gamepads or tracked wands<br>
<br>
<br>
<p align='middle'><img src='http://omegalib.googlecode.com/svn/wiki/intro/intro4.png' width='400' /><img src='http://omegalib.googlecode.com/svn/wiki/intro/intro3.png' width='400' /></p><p align='middle'>
omegalib supports <b>Python scripting</b> and integrates Mission Control, a tool to edit 3d scenes at runtime and <b>profile rendering and network performance</b>


<p align='middle'><img src='http://omegalib.googlecode.com/svn/wiki/intro/chem.jpg' width='800' /></p><p align='middle'>
<b>vtk</b> scientific visualization pipelines can be modified to run as omegalib applications<br>
<br>
<br>
<p align='middle'><img src='http://omegalib.googlecode.com/svn/wiki/intro/intro1.jpg' width='800' /></p><p align='middle'>
With omegalib, legacy applications can easily be converted to run on tiled display systems. <b>Yes, <a href='https://code.google.com/p/omegadoom/'>even Doom</a></b>.<br>
<br>
<br>
<p align='middle'>
<wiki:gadget url="http://www.ohloh.net/p/487998/widgets/project_basic_stats.xml" width="400" height="250" border="0"/></p>