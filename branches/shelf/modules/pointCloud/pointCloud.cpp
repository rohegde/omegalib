#include <omega.h>
#include <cyclops.h>
#include <omegaToolkit.h>
#include "PointsLoader.h"

using namespace omega;
using namespace cyclops;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Python wrapper code.
#ifdef OMEGA_USE_PYTHON
#include "omega/PythonInterpreterWrapper.h"
BOOST_PYTHON_MODULE(pointCloud)
{
	// PointsLoader
	PYAPI_REF_CLASS_WITH_CTOR(PointsLoader, ModelLoader);
}
#endif
