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

// Stdlib includes
#include <string>
#include <list>
#include <vector>

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
	typedef unsigned int uint;
	typedef unsigned long long uint64;
	typedef unsigned long long int64;
	typedef std::string String;

	// Container typedefs
	#ifdef __GNUC__
		template<typename K, typename T> class Dictionary: public std::tr1::unordered_map<K, T> {};
	#else
		template<typename K, typename T> class Dictionary: public stdext::hash_map<K, T> {};
	#endif
	template<typename T> class Vector: public std::vector<T> {};
	template<typename T> class List: public std::list<T> {};

	// Math and linear algebra typedefs
	typedef math::Math<float> Math;
	typedef math::Sphere<float> Sphere;
	typedef math::Plane<float> Plane;
	typedef math::Ray<float> Ray;
	typedef math::AlignedBox3<float> AlignedBox3;
	typedef math::matrix< 3, 3, float >  Matrix3f; //!< A 3x3 float matrix
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

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/** 
	 * 
	 * \brief Basefunctionality for IteratorWrappers
	 *
	 * 
	 * \param T a Container like vector list map ...
	 * \param IteratorType  T::iterator or T::const_iterator
	 * \param ValType  T::mapped_type in case of a map, T::value_type for vector, list,...
	 * 
	 * have a look at VectorIteratorWrapper and MapIteratorWrapper for a concrete usage
	*/
	//-------------------------------------------------------------------------------------------------
	// Original code taken from OGRE
	//  Copyright (c) 2000-2009 Torus Knot Software Ltd
	//  For the latest info, see http://www.ogre3d.org/
	template <typename T, typename IteratorType, typename ValType>
	class IteratorWrapper
	{

		private:
			/// Private constructor since only the parameterised constructor should be used
			IteratorWrapper();

		protected:
			IteratorType mBegin;
			IteratorType mCurrent;
			IteratorType mEnd;
	

		public:
	
			/// type you expect to get by funktions like peekNext(Value)
			typedef ValType ValueType;
			/// type you expect to get by funktions like peekNext(Value)Ptr
			typedef ValType* PointerType;

			/**
			\brief typedef to fulfill container interface
		
			Userfull if you want to use BOOST_boost_foreach
			\note there is no distinction between const_iterator and iterator.
			\n keep this in mind if you want to derivate from this class. 
			*/
			typedef IteratorType iterator;
		
			/**
			\brief typedef to fulfill container interface
		
			Userfull if you want to use BOOST_boost_foreach
			\note there is no distinction between const_iterator and iterator.
			\n keep this in mind if you want to derivate from this class. 
			*/
			typedef IteratorType const_iterator;
		
		
		

		
			/** Constructor.
			@remarks
			Provide a start and end iterator to initialise.
			*/
			IteratorWrapper ( IteratorType start, IteratorType last )
			: mBegin( start ), mCurrent ( start ), mEnd ( last )
			{
			}


			/** Returns true if there are more items in the collection. */
			bool hasMoreElements ( ) const
			{
				return mCurrent != mEnd;
			}


			/** Moves the iterator on one element. */
			void moveNext (  )
			{
				++mCurrent;
			}

			/** bookmark to the begin of the underlying collection */
			const IteratorType& begin() {return mBegin;}
		
		
			/** full access to the current  iterator */
			IteratorType& current(){return mCurrent;}
		
			/** bookmark to the end (one behind the last element) of the underlying collection */
			const IteratorType& end() {return mEnd;}
		

	};


	///////////////////////////////////////////////////////////////////////////////////////////////////
	/** 
	 * 
	 * \brief Prepiared IteratorWrapper for container like std::vector 
	 *
	 * 
	 * \param T = Container eg vector 
	 * \param IteratorType  T::iterator or T::const_iterator
	 * 
	 * have a look at VectorIterator and ConstVectorIterator for a more concrete usage
	*/
	//-------------------------------------------------------------------------------------------------
	// Original code taken from OGRE
	//  Copyright (c) 2000-2009 Torus Knot Software Ltd
	//  For the latest info, see http://www.ogre3d.org/
	template <typename T, typename IteratorType>
	class VectorIteratorWrapper : public IteratorWrapper<T, IteratorType, typename  T::value_type>
	{

		public:
			typedef typename IteratorWrapper<T, IteratorType, typename  T::value_type>::ValueType ValueType ; 
			typedef typename IteratorWrapper<T, IteratorType, typename  T::value_type>::PointerType PointerType ;
	

			/**
			 * \brief c'tor
			 * 
			 * Constructor that provide a start and end iterator to initialise.
			 * 
			 * @param start start iterator 
			 * @param end end iterator 
			 */
			VectorIteratorWrapper ( IteratorType start, IteratorType last )
			: IteratorWrapper<T, IteratorType, typename T::value_type>( start, last ) 
			{
			}


			/** Returns the next(=current) element in the collection, without advancing to the next. */
			ValueType peekNext (  ) const
			{
				return *(this->mCurrent);
			}

			/** Returns a pointer to the next(=current) element in the collection, without advancing to the next afterwards. */
			PointerType peekNextPtr (  )  const
			{
				return &(*(this->mCurrent) );
			}

			/** Returns the next(=current) value element in the collection, and advances to the next. */
			ValueType getNext (  ) 
			{
				return *(this->mCurrent++);
			}	

	};


	///////////////////////////////////////////////////////////////////////////////////////////////////
	/** 
	 * 
	 * \brief Concrete IteratorWrapper for nonconst access to the underlying container
	 * 
	 * \param T  Container 
	 * 
	*/
	//-------------------------------------------------------------------------------------------------
	// Original code taken from OGRE
	//  Copyright (c) 2000-2009 Torus Knot Software Ltd
	//  For the latest info, see http://www.ogre3d.org/
	template <typename T>
	class VectorIterator : public VectorIteratorWrapper<T,  typename T::iterator>{
		public:
			/** Constructor.
			@remarks
				Provide a start and end iterator to initialise.
			*/	
			VectorIterator( typename T::iterator start, typename T::iterator last )
			: VectorIteratorWrapper<T,  typename T::iterator>(start , last )
			{
			}

			/** Constructor.
			@remarks
				Provide a container to initialise.
			*/
			explicit VectorIterator( T& c )
			: VectorIteratorWrapper<T,  typename T::iterator> ( c.begin(), c.end() )
			{
			}
		
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/** 
	 * 
	 * \brief Concrete IteratorWrapper for const access to the underlying container
	 *
	 * 
	 * \param T = Container 
	 * 
	*/
	//-------------------------------------------------------------------------------------------------
	// Original code taken from OGRE
	//  Copyright (c) 2000-2009 Torus Knot Software Ltd
	//  For the latest info, see http://www.ogre3d.org/
	template <typename T>
	class ConstVectorIterator : public VectorIteratorWrapper<T,  typename T::const_iterator>{
		public:
			/** Constructor.
			@remarks
				Provide a start and end iterator to initialise.
			*/	
			ConstVectorIterator( typename T::const_iterator start, typename T::const_iterator last )
			: VectorIteratorWrapper<T,  typename T::const_iterator> (start , last )
			{
			}

			/** Constructor.
			@remarks
				Provide a container to initialise.
			*/
			explicit ConstVectorIterator ( const T& c )
			 : VectorIteratorWrapper<T,  typename T::const_iterator> (c.begin() , c.end() )
			{
			}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/** 
	 * 
	 * \brief Prepiared IteratorWrapper for key-value container
	 *
	 * 
	 * \param T  Container  (map - or also set )
	 * \param  IteratorType T::iterator or T::const_iterator
	 * 
	 * have a look at MapIterator and ConstMapIterator for a concrete usage
	*/
	//-------------------------------------------------------------------------------------------------
	// Original code taken from OGRE
	//  Copyright (c) 2000-2009 Torus Knot Software Ltd
	//  For the latest info, see http://www.ogre3d.org/
	template <typename T, typename IteratorType>
	class MapIteratorWrapper  : public IteratorWrapper<T, IteratorType, typename T::mapped_type>
	{

		public:
			/// redefined ValueType for a map/set
			typedef typename IteratorWrapper<T, IteratorType, typename  T::mapped_type>::ValueType ValueType ; 
			/// redefined PointerType for a map/set
			typedef typename IteratorWrapper<T, IteratorType, typename  T::mapped_type>::PointerType PointerType ;	
		
			///unused, just to make it clear that map/set::value_type is not a ValueType
			typedef typename T::value_type PairType ; 
			/// type you expect to get by funktions like peekNextKey
			typedef typename T::key_type KeyType;
        
			/** Constructor.
			@remarks
				Provide a start and end iterator to initialise.
			*/
			MapIteratorWrapper ( IteratorType start, IteratorType last )
			: IteratorWrapper<T, IteratorType, typename T::mapped_type>( start, last ) 
			{
			}

			/** Returns the next(=current) key element in the collection, without advancing to the next. */
			KeyType peekNextKey(void) const
			{
				return this->mCurrent->first;
			}


			/** Returns the next(=current) value element in the collection, without advancing to the next. */
			ValueType peekNextValue (  ) const
			{
				return this->mCurrent->second;
			}


			/** Returns a pointer to the next/current value element in the collection, without 
			advancing to the next afterwards. */
			const PointerType peekNextValuePtr (  )  const
			{
				return &(this->mCurrent->second);
			}


			/** Returns the next(=current) value element in the collection, and advances to the next. */
			ValueType getNext()
			{
				return ((this->mCurrent++)->second) ;
			}	
	

	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/** 
	 * 
	 * \brief Concrete IteratorWrapper for nonconst access to the underlying key-value container
	 *
	 * 
	 * \param T key-value container
	 * 
	*/
	//-------------------------------------------------------------------------------------------------
	// Original code taken from OGRE
	//  Copyright (c) 2000-2009 Torus Knot Software Ltd
	//  For the latest info, see http://www.ogre3d.org/
	template <typename T>
	class MapIterator : public MapIteratorWrapper<T,  typename T::iterator>{
		public:
	
			/** Constructor.
			@remarks
				Provide a start and end iterator to initialise.
			*/	
			MapIterator( typename T::iterator start, typename T::iterator last )
			: MapIteratorWrapper<T,  typename T::iterator>(start , last )
			{
			}
		
			/** Constructor.
			@remarks
				Provide a container to initialise.
			*/
			explicit MapIterator( T& c )
			: MapIteratorWrapper<T,  typename T::iterator> ( c.begin(), c.end() )
			{
			}
		
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/** 
	 * 
	 * \brief Concrete IteratorWrapper for const access to the underlying key-value container
	 *
	 * 
	 * \param T key-value container
	 * 
	*/
	//-------------------------------------------------------------------------------------------------
	// Original code taken from OGRE
	//  Copyright (c) 2000-2009 Torus Knot Software Ltd
	//  For the latest info, see http://www.ogre3d.org/
	template <typename T>
	class ConstMapIterator : public MapIteratorWrapper<T,  typename T::const_iterator>{
		public:
	
			/** Constructor.
			@remarks
				Provide a start and end iterator to initialise.
			*/	
			ConstMapIterator( typename T::const_iterator start, typename T::const_iterator last )
			: MapIteratorWrapper<T,  typename T::const_iterator> (start , last )
			{
			}

			/** Constructor.
			@remarks
				Provide a container to initialise.
			*/
			explicit ConstMapIterator ( const T& c )
			 : MapIteratorWrapper<T,  typename T::const_iterator> (c.begin() , c.end() )
			{
			}
	};
}; // namespace omega
#endif