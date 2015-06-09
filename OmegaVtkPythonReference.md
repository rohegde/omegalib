<p><b>Last revision:</b> ver. 4.0 - 12 March 2012</p>



# Module omegaVtk #
omegaVtk adds support for the visualization toolkit to omegalib


---

## Misc Functions ##

### `vtkAttachProp(actor, sceneNode)` ###
Attaches a vtk actor to an omegalib scene node

### `vtkDetachProp(actor, sceneNode)` ###
Detaches a vtk actor to an omegalib scene node

### `vtkAddLight(light)` ###
Adds a vtk light to the vtk scene. Light must be of `vtkLight` type.

### `vtkRemoveLight(light)` ###
Removes a vtk light to the vtk scene. Light must be of `vtkLight` type.

### `vtkRemoveAllLights()` ###
Removes all lights from the vtk scene.


---

## `class VtkModule` ##
Manages vtk rendering in omegalib
### Methods ###
| Method(s) | Description |
|:----------|:------------|
| `createAndInitialize` **static** | Creates and initializes the vtk support module. Call this before importing vtk and doing any vtk or omegalib operation. |

### Notes ###
`VtkModule` must be initialized **BEFORE** importing vtk. This is some typical initialization code for an omegalib/vtk app:
```
	# Import omegalib modules
	from omega import *
	from omegaToolkit import *
	from omegaVtk import *
	
	# Initialize vtk module
	vtkModule = VtkModule.createAndInitialize()

	# Import vtk stuff
	import vtk
	
	# Omegalib / vtk code goes here...
```