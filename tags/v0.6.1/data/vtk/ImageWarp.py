# This example shows how to combine data from both the imaging and
# graphics pipelines. The vtkMergeFilter is used to merge the data
# from each together.
import vtk
import ovtk

# Read in an image and compute a luminance value. The image is
# extracted as a set of polygons (vtkImageDataGeometryFilter). We then
# will warp the plane using the scalar (luminance) values.
reader = vtk.vtkBMPReader()
reader.SetFileName(ovtk.findFile("vtk/data/masonry.bmp"))
luminance = vtk.vtkImageLuminance()
luminance.SetInputConnection(reader.GetOutputPort())
geometry = vtk.vtkImageDataGeometryFilter()
geometry.SetInputConnection(luminance.GetOutputPort())
warp = vtk.vtkWarpScalar()
warp.SetInputConnection(geometry.GetOutputPort())
warp.SetScaleFactor(-0.1)

# Use vtkMergeFilter to combine the original image with the warped
# geometry.
merge = vtk.vtkMergeFilter()
merge.SetGeometry(warp.GetOutput())
merge.SetScalars(reader.GetOutput())
mapper = vtk.vtkDataSetMapper()
mapper.SetInputConnection(merge.GetOutputPort())
mapper.SetScalarRange(0, 255)
mapper.ImmediateModeRenderingOff()
actor = vtk.vtkActor()
actor.SetMapper(mapper)

# Add the actors to the renderer, set the background and size
ovtk.addActor(actor)
