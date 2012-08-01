# This simple example shows how to do basic texture mapping.
import vtk
import ovtk

# Load in the texture map. A texture is any unsigned char image. If it
# is not of this type, you will have to map it through a lookup table
# or by using vtkImageShiftScale.
bmpReader = vtk.vtkBMPReader()
bmpReader.SetFileName(ovtk.findFile("vtk/data/masonry.bmp"))
atext = vtk.vtkTexture()
atext.SetInputConnection(bmpReader.GetOutputPort())
atext.InterpolateOn()

# Create a plane source and actor. The vtkPlanesSource generates
# texture coordinates.
plane = vtk.vtkPlaneSource()
planeMapper = vtk.vtkPolyDataMapper()
planeMapper.SetInputConnection(plane.GetOutputPort())
planeActor = vtk.vtkActor()
planeActor.SetMapper(planeMapper)
planeActor.SetTexture(atext)

# Add the actors to the renderer, set the background and size
ovtk.addActor(planeActor)
