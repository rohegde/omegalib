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
 *---------------------------------------------------------------------------------------------------------------------
 * Original code taken from OGRE
 * Copyright (c) 2000-2009 Torus Knot Software Ltd
 *  For the latest info, see http://www.ogre3d.org/
 *---------------------------------------------------------------------------------------------------------------------
 * This file is based on material originally from:
 * Geometric Tools, LLC
 * Copyright (c) 1998-2010
 * Distributed under the Boost Software License, Version 1.0.
 * http://www.boost.org/LICENSE_1_0.txt
 * http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
 *********************************************************************************************************************/
#ifndef __PLANE_H__
#define __PLANE_H__

#include <vmmlib\axis_aligned_box.hpp>

namespace vmml {
	/** Defines a plane in 3D space.
        @remarks
            A plane is defined in 3D space by the equation
            Ax + By + Cz + D = 0
        @par
            This equates to a vector (the normal of the plane, whose x, y
            and z components equate to the coefficients A, B and C
            respectively), and a constant (D) which is the distance along
            the normal you have to go to move the plane back to the origin.
     */
	template<typename T>
    class plane
    {
    public:
        /** Default constructor - sets everything to 0.
        */
        inline plane ();
        inline plane (const plane& rhs);
        /** Construct a plane through a normal, and a distance to move the plane along the normal.*/
        inline plane (const vector<3,T>& rkNormal, float fConstant);
		/** Construct a plane using the 4 constants directly **/
		inline plane (float a, float b, float c, float d);
        inline plane (const vector<3,T>& rkNormal, const vector<3,T>& rkPoint);
        inline plane (const vector<3,T>& rkPoint0, const vector<3,T>& rkPoint1,
            const vector<3,T>& rkPoint2);

        /** The "positive side" of the plane is the half space to which the
            plane normal points. The "negative side" is the other half
            space. The flag "no side" indicates the plane itself.
        */
        enum side
        {
            NO_SIDE,
            POSITIVE_SIDE,
            NEGATIVE_SIDE,
            BOTH_SIDE
        };

        inline side getSide (const vector<3,T>& rkPoint) const;

        /**
        returns the side where the aligneBox is. the flag BOTH_SIDE indicates an intersecting box.
        one corner ON the plane is sufficient to consider the box and the plane intersecting.
        */
        inline side getSide (const AxisAlignedBox& rkBox) const;

        /** Returns which side of the plane that the given box lies on.
            The box is defined as centre/half-size pairs for effectively.
        @param centre The centre of the box.
        @param halfSize The half-size of the box.
        @returns
            POSITIVE_SIDE if the box complete lies on the "positive side" of the plane,
            NEGATIVE_SIDE if the box complete lies on the "negative side" of the plane,
            and BOTH_SIDE if the box intersects the plane.
        */
        inline side getSide (const vector<3,T>& centre, const vector<3,T>& halfSize) const;

        /** This is a pseudodistance. The sign of the return value is
            positive if the point is on the positive side of the plane,
            negative if the point is on the negative side, and zero if the
            point is on the plane.
            @par
            The absolute value of the return value is the true distance only
            when the plane normal is a unit length vector.
        */
        inline float getDistance (const vector<3,T>& rkPoint) const;

        /** Redefine this plane based on 3 points. */
        inline void redefine(const vector<3,T>& rkPoint0, const vector<3,T>& rkPoint1,
            const vector<3,T>& rkPoint2);

		/** Redefine this plane based on a normal and a point. */
		inline void redefine(const vector<3,T>& rkNormal, const vector<3,T>& rkPoint);

		/** Project a vector onto the plane. 
		@remarks This gives you the element of the input vector that is perpendicular 
			to the normal of the plane. You can get the element which is parallel
			to the normal of the plane by subtracting the result of this method
			from the original vector, since parallel + perpendicular = original.
		@param v The input vector
		*/
		inline vector<3,T> projectVector(const vector<3,T>& v) const;

        /** Normalises the plane.
            @remarks
                This method normalises the plane's normal and the length scale of d
                is as well.
            @note
                This function will not crash for zero-sized vectors, but there
                will be no changes made to their components.
            @returns The previous length of the plane's normal.
        */
        inline float normalise(void);

		vector<3,T> normal;
        float d;

        /// Comparison operator
        bool operator==(const plane<T>& rhs) const
        {
            return (rhs.d == d && rhs.normal == normal);
        }
        bool operator!=(const plane<T>& rhs) const
        {
            return (rhs.d != d && rhs.normal != normal);
        }
    };

	//-----------------------------------------------------------------------
	template<typename T> inline
	plane<T>::plane ()
	{
		normal = vector<3,T>::ZERO;
		d = 0.0;
	}
	//-----------------------------------------------------------------------
	template<typename T> inline
	plane<T>::plane (const plane<T>& rhs)
	{
		normal = rhs.normal;
		d = rhs.d;
	}
	//-----------------------------------------------------------------------
	template<typename T> inline
	plane<T>::plane (const vector<3,T>& rkNormal, float fConstant)
	{
		normal = rkNormal;
		d = -fConstant;
	}
	//---------------------------------------------------------------------
	template<typename T> inline
	plane<T>::plane (float a, float b, float c, float _d)
		: normal(a, b, c), d(_d)
	{
	}
	//-----------------------------------------------------------------------
	template<typename T> inline
	plane<T>::plane (const vector<3,T>& rkNormal, const vector<3,T>& rkPoint)
	{
		redefine(rkNormal, rkPoint);
	}
	//-----------------------------------------------------------------------
	template<typename T> inline
	plane<T>::plane (const vector<3,T>& rkPoint0, const vector<3,T>& rkPoint1,
		const vector<3,T>& rkPoint2)
	{
		redefine(rkPoint0, rkPoint1, rkPoint2);
	}
	//-----------------------------------------------------------------------
	template<typename T> inline
	float plane<T>::getDistance (const vector<3,T>& rkPoint) const
	{
		return normal.dot(rkPoint) + d;
	}
	//-----------------------------------------------------------------------
	template<typename T> inline
	typename plane<T>::side plane<T>::getSide (const vector<3,T>& rkPoint) const
	{
		float fDistance = getDistance(rkPoint);

		if ( fDistance < 0.0 )
			return plane<T>::NEGATIVE_SIDE;

		if ( fDistance > 0.0 )
			return plane<T>::POSITIVE_SIDE;

		return plane<T>::NO_SIDE;
	}


	//-----------------------------------------------------------------------
	template<typename T> inline
	typename plane<T>::side plane<T>::getSide (const AxisAlignedBox& box) const
	{
		if (box.isNull()) 
			return NO_SIDE;
		if (box.isInfinite())
			return BOTH_SIDE;

        return getSide(box.getCenter(), box.getHalfSize());
	}
    //-----------------------------------------------------------------------
	template<typename T> inline
    typename plane<T>::side plane<T>::getSide (const vector<3,T>& centre, const vector<3,T>& halfSize) const
    {
        // Calculate the distance between box centre and the plane
        float dist = getDistance(centre);

        // Calculate the maximise allows absolute distance for
        // the distance between box centre and plane
        float maxAbsDist = abs(normal.dot(halfSize));

        if (dist < -maxAbsDist)
            return plane<T>::NEGATIVE_SIDE;

        if (dist > +maxAbsDist)
            return plane<T>::POSITIVE_SIDE;

        return plane<T>::BOTH_SIDE;
    }
	//-----------------------------------------------------------------------
	template<typename T> inline
	void plane<T>::redefine(const vector<3,T>& rkPoint0, const vector<3,T>& rkPoint1,
		const vector<3,T>& rkPoint2)
	{
		vector<3,T> kEdge1 = rkPoint1 - rkPoint0;
		vector<3,T> kEdge2 = rkPoint2 - rkPoint0;
		normal = kEdge1.cross(kEdge2);
		normal.normalize();
		d = -normal.dot(rkPoint0);
	}
	//-----------------------------------------------------------------------
	template<typename T> inline
	void plane<T>::redefine(const vector<3,T>& rkNormal, const vector<3,T>& rkPoint)
	{
		normal = rkNormal;
		d = -rkNormal.dot(rkPoint);
	}
	//-----------------------------------------------------------------------
	//vector<3,T> plane<T>::projectVector(const vector<3,T>& p) const
	//{
	//	// We know plane normal is unit length, so use simple method
	//	Matrix3f xform;
	//	xform[0][0] = 1.0f - normal.x() * normal.x();
	//	xform[0][1] = -normal.x() * normal.y();
	//	xform[0][2] = -normal.x() * normal.z();
	//	xform[1][0] = -normal.y() * normal.x();
	//	xform[1][1] = 1.0f - normal.y() * normal.y();
	//	xform[1][2] = -normal.y() * normal.z();
	//	xform[2][0] = -normal.z() * normal.x();
	//	xform[2][1] = -normal.z() * normal.y();
	//	xform[2][2] = 1.0f - normal.z() * normal.z();
	//	return xform * p;

	//}

	//-----------------------------------------------------------------------
	template<typename T> inline
    float plane<T>::normalise(void)
    {
        float fLength = normal.length();

        // Will also work for zero-sized vectors, but will change nothing
        if (fLength > 1e-08f)
        {
            float fInvLength = 1.0f / fLength;
            normal *= fInvLength;
            d *= fInvLength;
        }

        return fLength;
    }
} // namespace omega

#endif
