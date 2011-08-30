# This example demonstrates how to extract "computational planes" from
# a structured dataset. Structured data has a natural, logical
# coordinate system based on i-j-k indices. Specifying imin,imax,
# jmin,jmax, kmin,kmax pairs can indicate a point, line, plane, or
# volume of data.
#
# In this example, we extract three planes and warp them using scalar
# values in the direction of the local normal at each point. This
# gives a sort of "velocity profile" that indicates the nature of the
# flow.

import vtk
import ovtk

# Here we read data from a annular combustor. A combustor burns fuel
# and air in a gas turbine (e.g., a jet engine) and the hot gas
# eventually makes its way to the turbine section.
pl3d = vtk.vtkPLOT3DReader()
pl3d.SetXYZFileName(ovtk.findFile("vtk/data/combxyz.bin"))
pl3d.SetQFileName(ovtk.findFile("vtk/data/combq.bin"))
pl3d.SetScalarFunctionNumber(100)
pl3d.SetVectorFunctionNumber(202)
pl3d.Update()

# Planes are specified using a imin,imax, jmin,jmax, kmin,kmax
# coordinate specification. Min and max i,j,k values are clamped to 0
# and maximum value.
plane = vtk.vtkStructuredGridGeometryFilter()
plane.SetInputConnection(pl3d.GetOutputPort())
plane.SetExtent(10, 10, 1, 100, 1, 100)
plane2 = vtk.vtkStructuredGridGeometryFilter()
plane2.SetInputConnection(pl3d.GetOutputPort())
plane2.SetExtent(30, 30, 1, 100, 1, 100)
plane3 = vtk.vtkStructuredGridGeometryFilter()
plane3.SetInputConnection(pl3d.GetOutputPort())
plane3.SetExtent(45, 45, 1, 100, 1, 100)

# We use an append filter because that way we can do the warping,
# etc. just using a single pipeline and actor.
appendF = vtk.vtkAppendPolyData()
appendF.AddInput(plane.GetOutput())
appendF.AddInput(plane2.GetOutput())
appendF.AddInput(plane3.GetOutput())
warp = vtk.vtkWarpScalar()
warp.SetInputConnection(appendF.GetOutputPort())
warp.UseNormalOn()
warp.SetNormal(1.0, 0.0, 0.0)
warp.SetScaleFactor(2.5)
normals = vtk.vtkPolyDataNormals()
normals.SetInput(warp.GetPolyDataOutput())
normals.SetFeatureAngle(60)
planeMapper = vtk.vtkPolyDataMapper()
planeMapper.SetInputConnection(normals.GetOutputPort())
planeMapper.SetScalarRange(pl3d.GetOutput().GetScalarRange())
planeActor = vtk.vtkActor()
planeActor.SetMapper(planeMapper)
planeActor.GetProperty().SetLighting(0)

# The outline provides context for the data and the planes.
outline = vtk.vtkStructuredGridOutlineFilter()
outline.SetInputConnection(pl3d.GetOutputPort())
outlineMapper = vtk.vtkPolyDataMapper()
outlineMapper.SetInputConnection(outline.GetOutputPort())
outlineActor = vtk.vtkActor()
outlineActor.SetMapper(outlineMapper)
outlineActor.GetProperty().SetColor(0, 0, 0)

ovtk.addActor(outlineActor)
ovtk.addActor(planeActor)
