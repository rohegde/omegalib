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
 *********************************************************************************************************************/
#ifndef __SPHERE_H_
#define __SPHERE_H_

#include "Math.h"

namespace omega { namespace math 
{
	/** A sphere primitive, mostly used for bounds checking. 
    @remarks
        A sphere in math texts is normally represented by the function
        x^2 + y^2 + z^2 = r^2 (for sphere's centered on the origin). Ogre stores spheres
        simply as a center point and a radius.
    */
	template<typename T>
    class Sphere
    {
    protected:
        T mRadius;
        vector<3,T> mCenter;
    public:
        /** Standard constructor - creates a unit sphere around the origin.*/
        Sphere() : mRadius(1.0), mCenter(vector<3,T>::Zero()) {}
        /** Constructor allowing arbitrary spheres. 
            @param center The center point of the sphere.
            @param radius The radius of the sphere.
        */
        Sphere(const vector<3,T>& center, T radius)
            : mRadius(radius), mCenter(center) {}

        /** Returns the radius of the sphere. */
        T getRadius(void) const { return mRadius; }

        /** Sets the radius of the sphere. */
        void setRadius(T radius) { mRadius = radius; }

        /** Returns the center point of the sphere. */
        const vector<3,T>& getCenter(void) const { return mCenter; }

        /** Sets the center point of the sphere. */
        void setCenter(const vector<3,T>& center) { mCenter = center; }

		/** Returns whether or not this sphere intersects another sphere. */
		bool intersects(const Sphere<T>& s) const
		{
            return (s.mCenter - mCenter).squaredLength() <=
                Math<T>::sqr(s.mRadius + mRadius);
		}
		/** Returns whether or not this sphere intersects a box. */
		bool intersects(const AlignedBox3<T>& box) const
		{
			return Math<T>::intersects(*this, box);
		}
		/** Returns whether or not this sphere intersects a plane. */
		bool intersects(const Plane<T>& plane) const
		{
			return Math<T>::intersects(*this, plane);
		}
		/** Returns whether or not this sphere intersects a point. */
		bool intersects(const vector<3,T>& v) const
		{
            return ((v - mCenter).squaredLength() <= Math<T>::sqr(mRadius));
		}
		/** Projects a point onto the sphere.
		@Returns: the coordinates of the projected point.*/
		vector<3,T> projectPoint(const vector<3,T> point)
		{
			vector<4,T> sphere = vector<4,T>(mCenter, mRadius);
			vector<3,T> result = sphere.projectOnSphere(point);
			return result;
		}
    };

}
}
#endif

