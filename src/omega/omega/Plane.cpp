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
#include "omega/Plane.h"

namespace omega
{
	//-----------------------------------------------------------------------
	Plane::Plane ()
	{
		normal = Vector3f::ZERO;
		d = 0.0;
	}
	//-----------------------------------------------------------------------
	Plane::Plane (const Plane& rhs)
	{
		normal = rhs.normal;
		d = rhs.d;
	}
	//-----------------------------------------------------------------------
	Plane::Plane (const Vector3f& rkNormal, float fConstant)
	{
		normal = rkNormal;
		d = -fConstant;
	}
	//---------------------------------------------------------------------
	Plane::Plane (float a, float b, float c, float _d)
		: normal(a, b, c), d(_d)
	{
	}
	//-----------------------------------------------------------------------
	Plane::Plane (const Vector3f& rkNormal, const Vector3f& rkPoint)
	{
		redefine(rkNormal, rkPoint);
	}
	//-----------------------------------------------------------------------
	Plane::Plane (const Vector3f& rkPoint0, const Vector3f& rkPoint1,
		const Vector3f& rkPoint2)
	{
		redefine(rkPoint0, rkPoint1, rkPoint2);
	}
	//-----------------------------------------------------------------------
	float Plane::getDistance (const Vector3f& rkPoint) const
	{
		return normal.dot(rkPoint) + d;
	}
	//-----------------------------------------------------------------------
	Plane::Side Plane::getSide (const Vector3f& rkPoint) const
	{
		float fDistance = getDistance(rkPoint);

		if ( fDistance < 0.0 )
			return Plane::NEGATIVE_SIDE;

		if ( fDistance > 0.0 )
			return Plane::POSITIVE_SIDE;

		return Plane::NO_SIDE;
	}


	//-----------------------------------------------------------------------
	Plane::Side Plane::getSide (const AxisAlignedBox& box) const
	{
		if (box.isNull()) 
			return NO_SIDE;
		if (box.isInfinite())
			return BOTH_SIDE;

        return getSide(box.getCenter(), box.getHalfSize());
	}
    //-----------------------------------------------------------------------
    Plane::Side Plane::getSide (const Vector3f& centre, const Vector3f& halfSize) const
    {
        // Calculate the distance between box centre and the plane
        float dist = getDistance(centre);

        // Calculate the maximise allows absolute distance for
        // the distance between box centre and plane
        float maxAbsDist = abs(normal.dot(halfSize));

        if (dist < -maxAbsDist)
            return Plane::NEGATIVE_SIDE;

        if (dist > +maxAbsDist)
            return Plane::POSITIVE_SIDE;

        return Plane::BOTH_SIDE;
    }
	//-----------------------------------------------------------------------
	void Plane::redefine(const Vector3f& rkPoint0, const Vector3f& rkPoint1,
		const Vector3f& rkPoint2)
	{
		Vector3f kEdge1 = rkPoint1 - rkPoint0;
		Vector3f kEdge2 = rkPoint2 - rkPoint0;
		normal = kEdge1.cross(kEdge2);
		normal.normalize();
		d = -normal.dot(rkPoint0);
	}
	//-----------------------------------------------------------------------
	void Plane::redefine(const Vector3f& rkNormal, const Vector3f& rkPoint)
	{
		normal = rkNormal;
		d = -rkNormal.dot(rkPoint);
	}
	//-----------------------------------------------------------------------
	//Vector3f Plane::projectVector(const Vector3f& p) const
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
    float Plane::normalise(void)
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
	//-----------------------------------------------------------------------
	std::ostream& operator<< (std::ostream& o, const Plane& p)
	{
		o << "Plane(normal=" << p.normal << ", d=" << p.d << ")";
		return o;
	}
}

