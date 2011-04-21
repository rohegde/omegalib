# This example reads a volume dataset, extracts an isosurface that
# represents the skin and displays it.
import vtk
import ovtk

# The following reader is used to read a series of 2D slices (images)
# that compose the volume. The slice dimensions are set, and the
# pixel spacing. The data Endianness must also be specified. The reader
# usese the FilePrefix in combination with the slice number to construct
# filenames using the format FilePrefix.%d. (In this case the FilePrefix
# is the root name of the file: quarter.)
v16 = vtk.vtkVolume16Reader()
v16.SetDataDimensions(64, 64)
v16.SetDataByteOrderToLittleEndian()

fileFormat = ovtk.findFile("vtk/data/headsq/quarter.1")
fileFormat = fileFormat.rstrip(".1");

v16.SetFilePrefix(fileFormat)
v16.SetImageRange(1, 93)
v16.SetDataSpacing(3.2, 3.2, 1.5)

# An isosurface, or contour value of 500 is known to correspond to the
# skin of the patient. Once generated, a vtkPolyDataNormals filter is
# is used to create normals for smooth surface shading during rendering.
# The triangle stripper is used to create triangle strips from the
# isosurface these render much faster on may systems.
skinExtractor = vtk.vtkContourFilter()
skinExtractor.SetInputConnection(v16.GetOutputPort())
skinExtractor.SetValue(0, 500)
skinNormals = vtk.vtkPolyDataNormals()
skinNormals.SetInputConnection(skinExtractor.GetOutputPort())
skinNormals.SetFeatureAngle(60.0)
skinMapper = vtk.vtkPolyDataMapper()
skinMapper.SetInputConnection(skinNormals.GetOutputPort())
skinMapper.ScalarVisibilityOff()
skin = vtk.vtkActor()
skin.SetMapper(skinMapper)

# An outline provides context around the data.
outlineData = vtk.vtkOutlineFilter()
outlineData.SetInputConnection(v16.GetOutputPort())
mapOutline = vtk.vtkPolyDataMapper()
mapOutline.SetInputConnection(outlineData.GetOutputPort())
outline = vtk.vtkActor()
outline.SetMapper(mapOutline)
outline.GetProperty().SetColor(0, 0, 0)

outline.SetScale(0.001)
skin.SetScale(0.001)


# Actors are added to the renderer. An initial camera view is created.
# The Dolly() method moves the camera towards the FocalPoint,
# thereby enlarging the image.
ovtk.addActor(outline)
ovtk.addActor(skin)
