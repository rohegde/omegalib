/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __OTYPES_H__
#define __OTYPES_H__

#include "osysdefs.h"

// Stdlib includes
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <time.h>

// boost includes
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#define NOMINMAX

// Unordered map: use different implementations on linux & windows.
#ifdef __GNUC__
	#include <tr1/unordered_map>
#else
	#include <hash_map>
#endif

// Libconfig
#include "libconfig/libconfig.hh"

// make sure the min and max macros are undefined.
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// eigenwrap includes
#include <omega/math/Math.h>
#include <omega/math/Ray.h>
#include <omega/math/AlignedBox.h>
#include <omega/math/Sphere.h>
#include <omega/math/Plane.h>

// Forward declarations of some OpenGL and OpenCL types.
// Using these, we can avoid including gl, cl (and windows) headers for every compiled source file.
// Source files that make explicit use of OpenGL/or OpenCL functionality have to include glheaders.h
struct _cl_program;
struct _cl_kernel;
struct _cl_context;
struct _cl_mem;
struct _cl_command_queue;
struct _cl_device_id;
typedef _cl_device_id* cl_device_id;
typedef _cl_program* cl_program;
typedef _cl_kernel* cl_kernel;
typedef _cl_mem* cl_mem;
typedef _cl_context* cl_context;
typedef _cl_command_queue* cl_command_queue;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

namespace boost { template<class Ch, class Tr, class Alloc> class basic_format; };

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	class DynamicObject
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Forward declarations
	class Application;

	// Basic typedefs
	typedef unsigned char byte;
	#ifndef OMEGA_OS_LINUX 
	typedef unsigned int uint;
	#endif
	typedef unsigned long long uint64;
	typedef unsigned long long int64;
	typedef std::string String;

	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename K, typename T>
	class KeyValue: public std::pair<K, T>
	{
	public:
		KeyValue(const K& k, T v): std::pair<K, T>(k, v) {}
		KeyValue(std::pair<K, T> src): std::pair<K, T>(src.first, src.second) {}
		KeyValue(std::pair<const K, T> src): std::pair<K, T>(src.first, src.second) {}
		const K& getKey() { return this->first; }
		T getValue() { return this->second; }
		T operator->() { return this->second; }
	};

	// Container typedefs
	#ifdef __GNUC__
		template<typename K, typename T> class Dictionary: public std::tr1::unordered_map<K, T> {
	#else
		template<typename K, typename T> class Dictionary: public stdext::hash_map<K, T> {
	#endif
		public:
			typedef KeyValue<K, T> Item;
			typedef std::pair< std::vector<T>::iterator, std::vector<T>::iterator> Range;
			typedef std::pair< std::vector<T>::const_iterator, std::vector<T>::const_iterator> ConstRange;
		};

	template<typename T> class Vector: public std::vector<T>
	{
	public:
		typedef std::pair< std::vector<T>::iterator, std::vector<T>::iterator> Range;
		typedef std::pair< std::vector<T>::const_iterator, std::vector<T>::const_iterator> ConstRange;
	};

	template<typename T> class List: public std::list<T> 
	{
	public:
		typedef std::pair< std::vector<T>::iterator, std::vector<T>::iterator> Range;
		typedef std::pair< std::vector<T>::const_iterator, std::vector<T>::const_iterator> ConstRange;
	};

	template<typename T> class Queue: public std::queue<T>
	{
	};

	// Math and linear algebra typedefs
	typedef math::Math<float> Math;
	typedef math::Sphere<float> Sphere;
	typedef math::Plane<float> Plane;
	typedef math::Ray<float> Ray;
	typedef math::AlignedBox3<float> AlignedBox3;
	typedef math::matrix< 3, 3, float >  Matrix3f; //!< A 3x3 float matrix
	typedef math::matrix< 4, 4, float >  Matrix4f; //!< A 3x3 float matrix
	typedef math::matrix< 3, Eigen::Dynamic, float >  Vectors3f; //!< A 3x3 float matrix
	typedef math::vector< 2, float > Vector2f; //!< A two-component float vector
	typedef math::vector< 3, float > Vector3f; //!< A three-component float vector
	typedef math::vector< 4, float > Vector4f; //!< A four-component float vector
	typedef math::vector< 2, int > Vector2i; //!< A four-component int vector
	typedef math::vector< 3, int > Vector3i; //!< A four-component int vector
	typedef math::vector< 4, int > Vector4i; //!< A four-component int vector
	typedef math::quaternion<float> Quaternion; //! A floating point quaternion
	typedef math::vector< 3, uint >      Triangle;
	typedef Eigen::AngleAxis<float> AngleAxis;
	typedef math::transform<3, float> Transform3;
	typedef math::transform<3, float, Eigen::Affine> AffineTransform3;
	typedef math::Rect Rect;

		// Misc typedefs
	typedef libconfig::Setting Setting;


	//! enumeration for the axes
	enum Axis
	{
		AxisX,
		AxisY,
		AxisZ
	};

	//! Enumeration for orientation.
	enum Orientation 
	{
		Horizontal = 0, 
		Vertical = 1
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class Stopwatch
	{
	public:
		Stopwatch():
		  myIsRunning(false), myLt(0), mySt(0) {}

		void start()
		{
			mySt = (float)((double)clock() / CLOCKS_PER_SEC);
			myLt = mySt;
			myIsRunning = true;
		}
		void stop()
		{
			myIsRunning = false;
		}
		bool isRunning()
		{
		}
		float getElapsedTime()
		{
			float t = (float)((double)clock() / CLOCKS_PER_SEC);
			return t - myLt;
		}
		float getDt()
		{
			float t = (float)((double)clock() / CLOCKS_PER_SEC);
			float dt = myLt - t;
			myLt = t;
			return dt;
		}
	private:
		bool myIsRunning;
		float mySt;
		float myLt;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//! Utility class to generate a sequentially numbered series of names
	//-------------------------------------------------------------------------------------------------
	// Original code taken from OGRE
	//  Copyright (c) 2000-2009 Torus Knot Software Ltd
	//  For the latest info, see http://www.ogre3d.org/
	class NameGenerator
	{
	protected:
		String mPrefix;
		unsigned long long int mNext;
		//OGRE_AUTO_MUTEX
	public:
		NameGenerator(const NameGenerator& rhs)
			: mPrefix(rhs.mPrefix), mNext(rhs.mNext) {}
		
		NameGenerator(const String& prefix) : mPrefix(prefix), mNext(1) {}

		/// Generate a new name
		String generate()
		{
			//OGRE_LOCK_AUTO_MUTEX
			std::ostringstream s;
			s << mPrefix << mNext++;
			return s.str();
		}

		/// Reset the internal counter
		void reset()
		{
			//OGRE_LOCK_AUTO_MUTEX
			mNext = 1ULL;
		}

		/// Manually set the internal counter (use caution)
		void setNext(unsigned long long int val)
		{
			//OGRE_LOCK_AUTO_MUTEX
			mNext = val;
		}

		/// Get the internal counter
		unsigned long long int getNext() const
		{
			// lock even on get because 64-bit may not be atomic read
			//OGRE_LOCK_AUTO_MUTEX
			return mNext;
		}
	};
}; // namespace omega
#endif
