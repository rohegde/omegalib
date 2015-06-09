# Vtk programming in omegalib #
<p><b>Last revision:</b> ver. 2.1 - 27 June 2012</p>

On this page you will learn how to put together a basic omegalib application to render a visualization toolkit pipeline. [The visualization toolkit (vtk)](http://www.vtk.org) is a standard open source framework for scientific and information visualization. If you have never heard of it yo can probably skip this page, unless you want to get some additional information on the inner workings of omegalib.

## omega/vtk application structure ##
<p align='middle'><img src='http://omegalib.googlecode.com/svn/wiki/BasicVtk/pipeline.png' width='500' /></p><p align='middle'>
<i><sup>The components of a vtk pipeline running inside omegalib.</sup></i>
</p>

In order to make vtk work inside an omegalib application, we need to split the vtk pipeline in two parts: the **CPU-bound part** and the **GPU-bound part**. The CPU buound part, mostly made up by vtk filters, runs unmodified inside your main application class.
But GPU-bound objects like mappers and actors require a graphics context to run. For this reason, they need to be duplicated for each pipe used by your application. Most of this work is done automatically by the omegaVtk module, but you will need some minimal level of bookkeeping in your code.

## The vtkhello application ##
**NOTE:** the vtkhello application is build only if you enabled vtk support during the omegalib build, and you have a version of vtk installed. read the [Building](Building.md) page for more information.
```
	#include <vtkSphereSource.h>
	#include <vtkPolyDataMapper.h>
	#include <vtkProperty.h>
	#include <vtkActor.h>

	#include <omega.h>
	#include <omegaToolkit.h>
	#include <omegaVtk.h>

	using namespace omega;
	using namespace omegaToolkit;
	using namespace omegaVtk;

	class VtkScene: public EngineModule
	{
	public:
		VtkScene();
		virtual void initialize();
		virtual void initializeRenderer(Renderer* r);

	private:
		VtkModule* myVtkModule;
		SceneNode* mySceneNode;
		DefaultMouseInteractor* myMouseInteractor;
		vtkSphereSource* mySphere;

		// Client objecs.
		RendererObject<vtkPolyDataMapper*> myPolyDataMapper;
		RendererObject<vtkActor*> myActor;
	};

	VtkScene::VtkScene() 
	{
		// Create and register the omegalib vtk module.
		myVtkModule = new VtkModule();
		ModuleServices::addModule(myVtkModule);
	}

	void VtkScene::initialize()
	{
		// As simple as it gets: create a sphere in vtk.
		mySphere = vtkSphereSource::New(); 
		mySphere->SetRadius(0.20); 
		mySphere->SetThetaResolution(18); 
		mySphere->SetPhiResolution(18);

		// Create an omegalib scene node. We will attach our vtk objects to it.
		mySceneNode = new SceneNode(getServer(), "vtkRoot");
		mySceneNode->setPosition(0, 0, -1);
		mySceneNode->setBoundingBoxVisible(true);
		getServer()->getScene()->addChild(mySceneNode);

		// Create a mouse interactor and associate it with our scene node.
		myMouseInteractor = new DefaultMouseInteractor();
		myMouseInteractor->setSceneNode(mySceneNode);
		ModuleServices::addModule(myMouseInteractor);

		// Setup the camera
		getServer()->getDefaultCamera()->focusOn(getServer()->getScene());
	}

	void VtkScene::initializeRenderer(Renderer* r)
	{
		// For each omegalib renderer thread, create vtk polydata mappers and actors.
		myPolyDataMapper[r] = vtkPolyDataMapper::New();
		myPolyDataMapper[r]->SetInput(mySphere->GetOutput());
		myActor[r] = vtkActor::New(); 
		myActor[r]->SetMapper(myPolyDataMapper[r]); 
		myActor[r]->GetProperty()->SetColor(0,0,1);

		// Here is where the magic happens: use the omegalib vtk module to attach the vtk actor to the
		// node we created in initialize. 
		myVtkModule->beginClientInitialize(r);
		myVtkModule->attachProp(myActor[r], mySceneNode);
		myVtkModule->endClientInitialize();
	}
	
	int main(int argc, char** argv)
	{
		Application<VtkScene> app("vtkhello");
		return omain(app, argc, argv);
	}
```

<p align='middle'><img src='http://omegalib.googlecode.com/svn/wiki/BasicVtk/vtkhello.png' width='500' /></p><p align='middle'>
<i><sup>Not very fancy, but if you see this your omega/vtk application works.</sup></i>
</p>

If you read the other guides, The main application class (`VtkScene`) should be familiar by now. In the constructor, we create and initialize a `VtkModule` instance. We also reimplement the `initializeRenderer` method, this time not to add a custom render pass, but to **initialize our renderer-side objects** (the vtkActor and vtkPolyData instances). We keep the renderer-side instances inside the main appication class, using the [omega::RendererObject](http://omegalib.googlecode.com/svn/refdocs/trunk/html/classomega_1_1_renderer_object.html) utility class. This class is basically just a dictionary with `Renderer` pointers as keys. After creating the `vtkActor` instances, these lines...
```
	myVtkModule->beginClientInitialize(r);
	myVtkModule->attachProp(myActor[r], mySceneNode);
	myVtkModule->endClientInitialize();
```
...attach the actors to a scene node, making sure they will be displayed in the scene. Also note that in the application class contstructor we do:
```
	myMouseInteractor = new DefaultMouseInteractor();
	myMouseInteractor->setSceneNode(mySceneNode);
	ModuleServices::addModule(myMouseInteractor);
```
The previous code creates an interactor to manipulate scene nodes using your mouse, and attaches the scene node to it. This way, we will be able to move and rotate our vtk object using our mouse. Omegalib ships with a few different interactor classes you can use. **See also:** [omegaToolkit::ControllerManipulator](http://omegalib.googlecode.com/svn/refdocs/trunk/html/classomega_toolkit_1_1_controller_manipulator.html), [omegaToolkit::DefaultMouseInteractor](http://omegalib.googlecode.com/svn/refdocs/trunk/html/classomega_toolkit_1_1_default_mouse_interactor.html), [omegaToolkit::DefaultTwoHandsInteractor](http://omegalib.googlecode.com/svn/refdocs/trunk/html/classomega_toolkit_1_1_default_two_hands_interactor.html).