import vtk
import ovtk

quadric = vtk.vtkQuadric()
quadric.SetCoefficients(.5,1,.3,0,.1,0,0,.2,0,0)

sample = vtk.vtkSampleFunction()
sample.SetSampleDimensions(20,20,20)
sample.SetImplicitFunction(quadric)

contours = vtk.vtkContourFilter()
contours.SetInput(sample.GetOutput())
contours.GenerateValues(3, 0.0, 1.2)

contMapper = vtk.vtkPolyDataMapper()
contMapper.SetInput(contours.GetOutput())
contMapper.SetScalarRange(0.0, 1.2)

actor = vtk.vtkActor()
actor.SetScale(0.2)
actor.SetMapper(contMapper)

ovtk.addActor(actor)

