# This example demonstrates the use of vtkAssembly.  In an assembly,
# the motion of one actor affects the position of other actors.
import vtk
import ovtk

# Create four parts: a top level assembly (in this case, a
# vtkCylinder) and three primitives (using vtkSphereSource,
# vtkCubeSource, and vtkConeSource).  Set up mappers and actors for
# each part of the assembly to carry information about material
# properties and associated geometry.
sphere = vtk.vtkSphereSource()
sphereMapper = vtk.vtkPolyDataMapper()
sphereMapper.SetInputConnection(sphere.GetOutputPort())
sphereActor = vtk.vtkActor()
sphereActor.SetMapper(sphereMapper)
sphereActor.SetOrigin(2, 1, 3)
sphereActor.RotateY(6)
sphereActor.SetPosition(2.25, 0, 0)
sphereActor.GetProperty().SetColor(1, 0, 1)

cube = vtk.vtkCubeSource()
cubeMapper = vtk.vtkPolyDataMapper()
cubeMapper.SetInputConnection(cube.GetOutputPort())
cubeActor = vtk.vtkActor()
cubeActor.SetMapper(cubeMapper)
cubeActor.SetPosition(0.0, .25, 0)
cubeActor.GetProperty().SetColor(0, 0, 1)

cone = vtk.vtkConeSource()
coneMapper = vtk.vtkPolyDataMapper()
coneMapper.SetInputConnection(cone.GetOutputPort())
coneActor = vtk.vtkActor()
coneActor.SetMapper(coneMapper)
coneActor.SetPosition(0, 0, .25)
coneActor.GetProperty().SetColor(0, 1, 0)

# top part of the assembly
cylinder = vtk.vtkCylinderSource()
cylinderMapper = vtk.vtkPolyDataMapper()
cylinderMapper.SetInputConnection(cylinder.GetOutputPort())
cylinderMapper.SetResolveCoincidentTopologyToPolygonOffset()
cylinderActor = vtk.vtkActor()
cylinderActor.SetMapper(cylinderMapper)
cylinderActor.GetProperty().SetColor(1, 0, 0)

# Create the assembly and add the 4 parts to it.  Also set the origin,
# position and orientation in space.
assembly = vtk.vtkAssembly()
assembly.AddPart(cylinderActor)
assembly.AddPart(sphereActor)
assembly.AddPart(cubeActor)
assembly.AddPart(coneActor)
assembly.SetOrigin(5, 10, 15)
assembly.AddPosition(5, 0, 0)
assembly.RotateX(15)

ovtk.addActor(assembly)