# Mixing Python and C++ #
<p><b>Last revision:</b> ver. 3.7 - 20 April 2012<font color='#ff0000'> (This is a preliminary / work in progress document and may be incomplete)</font>
</p>

When needed you can build part of your program using C++ and turn that into a **python module** integrating the omegalib API. You can then import this module into python using `import moduleName` and use some of its exposed interfaces. This guide is divided into two parts: a guide on how to create a CMake configuation to build a CMake configuration to build the module, and how to expose your custom C++ code to python.

To learn how to use the C++ omegalib API, check out the programming guides on the [Getting Started Page](GettingStarted.md).

## Python Module CMake configuration ##
The following is an example of a CMakeLists.txt file that creates an omegalib python module:
```
	cmake_minimum_required(VERSION 2.8)
	  
	find_package(Omegalib REQUIRED)
	include_directories(${OMEGA_INCLUDE_DIRS})

	# Set the module name here
	set(MODULE_NAME templateModule)

	# Set module name and source files here
	add_library(${MODULE_NAME} SHARED 
		templateModule.cpp)

	# Set the module library dependencies here
	target_link_libraries(${MODULE_NAME}
		${OMEGA_LIB} 
		${OMEGA_TOOLKIT_LIB} 
		${CYCLOPS_LIB}) 

	#------------------------------------------------------------------------------
	# DO NOT MODIFY ANYTHING BELOW THIS LINE
	set_target_properties(${MODULE_NAME} PROPERTIES PREFIX "")
	if(WIN32)
		set_target_properties(${MODULE_NAME} PROPERTIES FOLDER modules SUFFIX ".pyd")
	endif()
```
Assuming you have an omegalib installation on our machine, you can find it using the CMake command `find_package(Omegalib)`. The command will add a new variable to your cmake configuration asking for the path to the omegalib build dir. After setting that, you will have a set of standard variables defined (`OMEGA_INCLUDE_DIRS`, `OMEGA_LIB`, `OMEGA_TOOLKIT_LIB`, `CYCLOPS_LIB`) that you can use to set header files and libraries for your project).

After building, the project will output a .so or .pyd file (depending on the platform). You will be able to import this module into a python omegalib script launcher through orun.

## Python Module CMake configuration ##
C++ classes and methods are exposed using a **module declaration** section in your source file. The module declaration lists all the classes and methods you want to expose, together with some minimal information about how they work. Here is an example of a module declaration:
```
	BOOST_PYTHON_MODULE(templateModule)
	{
		// SceneLoader
		PYAPI_REF_BASE_CLASS(HelloModule)
			PYAPI_STATIC_REF_GETTER(HelloModule, createAndInitialize)
			PYAPI_METHOD(HelloModule, setObjectColor)
			;
	}
```
The module declaration uses a set of macros that internally use Boost::python to interface python and C++. The previous declaration creates a module named `templateModule`, containing a class `HelloModule` with a static and nonstatic method.

### Python Wrapper Reference ###
Here is the set of macros that can be used to create a module declaration:
#### Class and Module Declarations ####
| **Macro Name** | **Description** |
|:---------------|:----------------|
| `BOOST_PYTHON_MODULE(name)` | Starts the module declaration block. Each library can have only one module declaration bloc. The module name **MUST** match the name of the output module file (for instance, in the previous example the module output should be `templateModule.pyd`) |
| `PYAPI_BASE_CLASS(name)` | Starts a base class declaration. This class will have **pass by value** semantics (i.e. `a = b` will make a copy of bject `b`) |
| `PYAPI_BASE_CLASS_WITH_CTOR(name)` | Like `PYAPI_BASE_CLASS` but the class has a default constructor, so objects can be constructed with `a = Class()`. |
| `PYAPI_REF_BASE_CLASS(name)` | Starts a base class declaration. This class will have **pass by reference** semantics (i.e. `a = b` will incrase the reference count of b, but both variable will refer to the same object) |
| `PYAPI_REF_BASE_CLASS_WITH_CTOR(name)` | Like `PYAPI_REF_BASE_CLASS` but the class has a default constructor, so objects can be constructed with `a = Class()`. |
| `PYAPI_REF_CLASS(name, base)` | Creates a pass-by-reference class derived from another class `base`. |
| `PYAPI_REF_CLASS_WITH_CTOR(name, base)` | Creates a pass-by-reference class derived from another class `base` and supporting a default constructor |

#### Method Declarations ####
Inside a class declaration you can use the following macros:
| **Macro Name** | **Description** |
|:---------------|:----------------|
| `PYAPI_METHOD(className, methodName)` | Declares a method for the class. The method can return void, or simple data types (`int`, `string`, `bool`, `float`), and can accept any number or arguments |
| `PYAPI_GETTER(className, methodName)` | Declares a getter for the class. The getter returns a user defined return-by-value type (see `PYAPI_BASE_CLASS`) or one built-in complex value type, like `Vector3`, `Quaternion`, etc. |
| `PYAPI_REF_GETTER(className, methodName)` | Declares a reference getter for the class. The getter returns a user defined return-by-reference type (see `PYAPI_REF_BASE_CLASS`) or one built-in reference type, like `SceneNode`, `Light`, etc. |
| `PYAPI_STATIC_REF_GETTER(className, methodName)` | Declares a **static** reference getter, that is, a reference getter that can be accessed using the class name directly (`b = Class.staticMethod()`) |
| `PYAPI_PROPERTY(className, methodName)` | Declares a property: a public class field that can be read and written from python. Use sparingly. |

#### Other Declarations ####
| **Macro Name** | **Description** |
|:---------------|:----------------|
| `PYAPI_ENUM(fullEnumName, enumName)` | Wraps a C++ enumeration into a Python enumeration. |
| `PYAPI_ENUM_VALUE(fullEnumName, enumName)` | Declares a value for the enumeration. |