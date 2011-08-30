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
#ifndef __RAY_H_
#define __RAY_H_

#include "Plane.h"
#include "PlaneBoundedVolume.h"

namespace omega { namespace math 
{
	/** Representation of a ray in space, i.e. a line with an origin and direction. */
	template<typename T>
    class Ray
    {
    protected:
        vector<3,T> mOrigin;
        vector<3,T> mDirection;
    public:
        Ray():mOrigin(vector<3,T>::Zero()), mDirection(vector<3,T>::UnitZ()) {}
        Ray(const vector<3,T>& origin, const vector<3,T>& direction)
            :mOrigin(origin), mDirection(direction) {}

        /** Sets the origin of the ray. */
        void setOrigin(const vector<3,T>& origin) {mOrigin = origin;} 
        /** Gets the origin of the ray. */
        const vector<3,T>& getOrigin(void) const {return mOrigin;} 

        /** Sets the direction of the ray. */
        void setDirection(const vector<3,T>& dir) {mDirection = dir;} 
        /** Gets the direction of the ray. */
        const vector<3,T>& getDirection(void) const {return mDirection;} 

		/** Gets the position of a point t units along the ray. */
		vector<3,T> getPoint(float t) const { 
			return vector<3,T>(mOrigin + (mDirection * t));
		}
		
		/** Gets the position of a point t units along the ray. */
		vector<3,T> operator*(float t) const { 
			return getPoint(t);
		}

		/** Tests whether this ray intersects the given plane. 
		@returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
		*/
		std::pair<bool, float> intersects(const Plane<T>& p) const
		{
			return Math<T>::intersects(*this, p);
		}
        /** Tests whether this ray intersects the given plane bounded volume. 
        @returns A pair structure where the first element indicates whether
        an intersection occurs, and if true, the second element will
        indicate the distance along the ray at which it intersects. 
        This can be converted to a point in space by calling getPoint().
        */
        std::pair<bool, float> intersects(const PlaneBoundedVolume<T>& p) const
        {
            return Math<T>::intersects(*this, p.planes, p.outside == Plane<T>::POSITIVE_SIDE);
        }
		/** Tests whether this ray intersects the given sphere. 
		@returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
		*/
		std::pair<bool, float> intersects(const Sphere<T>& s) const
		{
			return Math<T>::intersects(*this, s);
		}
		/** Tests whether this ray intersects the given box. 
		@returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
		*/
		std::pair<bool, float> intersects(const AlignedBox3<T>& box) const
		{
			return Math<T>::intersects(*this, box);
		}

		/** Computes the projection of a point.
		@returns the distance
		*/
		vector<3,T> projectPoint(const vector<3,T>& point) const
		{
			const vector<3,T>& v = mDirection;
			const vector<3,T> w = point - mOrigin;
			
			float c1 = w.dot(v);
			float c2 = v.dot(v);
			float b = c1 / c2;
			 vector<3,T> pb = mOrigin + b * v;
			 return pb;
		}
    };
	/** @} */
	/** @} */
}
}
#endif
