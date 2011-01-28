/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#ifndef __OTYPES_H__
#define __OTYPES_H__

typedef unsigned char byte;
typedef unsigned int uint;

// Stdlib includes
#include <string>

// vmmlib includes
#define VMMLIB_DONT_FORCE_ALIGNMENT
#include <vmmlib/vmmlib.hpp>

// Libconfig
#include "libconfig/libconfig.hh"

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class Application;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type definitions.
typedef vmml::matrix< 3, 3, float >  Matrix3f; //!< A 3x3 float matrix
typedef vmml::matrix< 4, 4, float >  Matrix4f; //!< A 4x4 float matrix
typedef vmml::vector< 2, int > Vector2i; //!< A two-component integer vector
typedef vmml::vector< 3, int > Vector3i; //!< A three-component integer vector
typedef vmml::vector< 4, int > Vector4i; //!< A four-component integer vector
typedef vmml::vector< 2, float > Vector2f; //!< A two-component float vector
typedef vmml::vector< 3, float > Vector3f; //!< A three-component float vector
typedef vmml::vector< 4, float > Vector4f; //!< A four-component float vector
typedef vmml::quaternion<float> Quaternion; //! A floating point quaternion
typedef vmml::vector< 3, uint >      Triangle;

//! enumeration for the axes
enum Axis
{
    AxisX,
    AxisY,
    AxisZ
};

// String
typedef std::string String;

// Dictionary 
#define Dictionary boost::unordered_map 

// Setting
typedef libconfig::Setting Setting;
typedef co::base::uint128_t uint128_t;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! wrapper to enable array use where arrays would not be allowed otherwise
template< class T, size_t d >
struct ArrayWrapper
{
    T& operator[]( const size_t i )
    {
        assert( i < d );
        return data[i];
    }
        
    const T& operator[]( const size_t i ) const
    {
        assert( i < d );
        return data[i];
    }
        
private:
    T data[d];
};
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Utility class to generate a sequentially numbered series of names
//---------------------------------------------------------------------------------------------------------------------
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
//---------------------------------------------------------------------------------------------------------------------
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
//---------------------------------------------------------------------------------------------------------------------
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * 
 * \brief Concrete IteratorWrapper for nonconst access to the underlying container
 * 
 * \param T  Container 
 * 
*/
//---------------------------------------------------------------------------------------------------------------------
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * 
 * \brief Concrete IteratorWrapper for const access to the underlying container
 *
 * 
 * \param T = Container 
 * 
*/
//---------------------------------------------------------------------------------------------------------------------
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
//---------------------------------------------------------------------------------------------------------------------
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * 
 * \brief Concrete IteratorWrapper for nonconst access to the underlying key-value container
 *
 * 
 * \param T key-value container
 * 
*/
//---------------------------------------------------------------------------------------------------------------------
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * 
 * \brief Concrete IteratorWrapper for const access to the underlying key-value container
 *
 * 
 * \param T key-value container
 * 
*/
//---------------------------------------------------------------------------------------------------------------------
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