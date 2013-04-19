#include <omega.h>
#include <cyclops.h>
#include <omegaToolkit.h>

using namespace omega;
using namespace cyclops;
using namespace omegaToolkit;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Module declaration
class HelloModule: public EngineModule
{
public:
	static HelloModule* createAndInitialize(SceneManager* sceneMng);
	HelloModule(SceneManager* sceneMng);

	virtual void initialize();
	virtual void update(const UpdateContext& context);

	//! Demo method to change the displayed object color.
	void setObjectColor(const String& color);

private:
	Ref<SceneManager> mySceneManager;
	Ref<StaticObject> myObject;
	Ref<PlaneShape> myPlane;
	Ref<Light> myLight;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Python wrapper code.
#ifdef OMEGA_USE_PYTHON
#include "omega/PythonInterpreterWrapper.h"
BOOST_PYTHON_MODULE(templateModule)
{
	// SceneLoader
	PYAPI_REF_BASE_CLASS(HelloModule)
		PYAPI_STATIC_REF_GETTER(HelloModule, createAndInitialize)
		PYAPI_METHOD(HelloModule, setObjectColor)
		;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
HelloModule* HelloModule::createAndInitialize(SceneManager* sceneMng)
{
	HelloModule* instance = new HelloModule(sceneMng);
	ModuleServices::addModule(instance);
	instance->doInitialize(Engine::instance());
	return instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
HelloModule::HelloModule(SceneManager* sceneMng):
	EngineModule("HelloModule"),
	mySceneManager(sceneMng)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HelloModule::initialize()
{
	// Load some basic model from the omegalib meshes folder.
	// Force the size of the object to be 0.8 meters
	//ModelInfo torusModel("simpleModel", "demos/showcase/data/walker/walker.fbx", 2.0f);
	ModelInfo* torusModel = new ModelInfo("simpleModel", "cyclops/test/torus.fbx", 2.0f);
	mySceneManager->loadModel(torusModel);

	// Create a new object using the loaded model (referenced using its name, 'simpleModel')
	myObject = new StaticObject(mySceneManager, "simpleModel");
	myObject->setName("object");
	myObject->setEffect("colored -d #303030 -g 1.0 -s 20.0 -v envmap");
	myObject->pitch(-90 * Math::DegToRad);
	myObject->setPosition(0, 0, -2);

	// Create a plane for reference.
	myPlane = new PlaneShape(mySceneManager, 100, 100, Vector2f(50, 50));
	myPlane->setName("ground");
	//plane->setEffect("textured -d cyclops/test/checker2.jpg -s 20 -g 1 -v envmap");
	myPlane->setEffect("textured -d cubemaps/gradient1/negy.png");
	myPlane->pitch(-90 * Math::DegToRad);
	myPlane->setPosition(0, 0, -2);

	// Setup a light for the scene.
	myLight = new Light(mySceneManager);
	myLight->setEnabled(true);
	myLight->setPosition(Vector3f(0, 5, -2));
	myLight->setColor(Color(1.0f, 1.0f, 1.0f));
	myLight->setAmbient(Color(0.1f, 0.1f, 0.1f));
	//light->setSoftShadowWidth(0.01f);
	mySceneManager->setMainLight(myLight);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HelloModule::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void HelloModule::setObjectColor(const String& color)
{
	myObject->setEffect(ostr("colored -d %1%", %color));
}
