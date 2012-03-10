import vtk
import omegaVtk

quadric = vtk.vtkQuadric()
quadric.SetCoefficients(.5,1,.2,0,.1,0,0,.2,0,0)

sample = vtk.vtkSampleFunction()
sample.SetSampleDimensions(50,50,50)
sample.SetImplicitFunction(quadric)

contours = vtk.vtkContourFilter()
contours.SetInput(sample.GetOutput())
contours.GenerateValues(6, 0.0, 2)

normals = vtk.vtkPolyDataNormals()
normals.SetInputConnection(contours.GetOutputPort())
normals.SetFeatureAngle(270.0)

omegaVtk.queueInitializeView()

def initializeView():
    contMapper = vtk.vtkPolyDataMapper()
    contMapper.SetInput(normals.GetOutput())
    contMapper.SetScalarRange(0.0, 1.2)
    contMapper.SetScalarVisibility(1)
    actor = vtk.vtkActor()
    actor.SetMapper(contMapper)
    vtkRoot = nodeGetChildByName(nodeGetRoot(), "vtkRoot")
    omegaVtk.attachProp(actor, vtkRoot)
	#ovtk.addCheckButton("testButton", 'contMapper.GetScalarVisibility()', 'contMapper.SetScalarVisibility(${value})')
	#ovtk.addSlider("slider", 1, 10, 1, '6', 'contours.GenerateValues(${value}, 0, 2)')


