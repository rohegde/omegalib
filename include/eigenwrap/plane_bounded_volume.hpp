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
#ifndef __PLANE_BOUNDED_VOLUME_H_
#define __PLANE_BOUNDED_VOLUME_H_

#include <eigenwrap/axis_aligned_box.hpp>
#include <eigenwrap/axis_aligned_box.hpp>
#include <eigenwrap/math.hpp>

namespace eigenwrap
{
	/** Represents a convex volume bounded by planes.
    */
	template<typename T>
    class plane_bounded_volume
    {
    public:
        typedef std::vector<plane<T> > PlaneList;
        /// Publicly accessible plane list, you can modify this direct
        PlaneList planes;
        plane_side outside;

        plane_bounded_volume() :outside(NEGATIVE_SIDE) {}
        /** Constructor, determines which side is deemed to be 'outside' */
        plane_bounded_volume(plane_side theOutside) 
            : outside(theOutside) {}

        /** Intersection test with AABB
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const axis_aligned_box<T>& box) const
        {
            if (box.isNull()) return false;
            if (box.isInfinite()) return true;

            // Get centre of the box
            vector<3,T> centre = box.getCenter();
            // Get the half-size of the box
            vector<3,T> halfSize = box.getHalfSize();
            
            typename PlaneList::const_iterator i, iend;
            iend = planes.end();
            for (i = planes.begin(); i != iend; ++i)
            {
                const plane<T> & plane = *i;

                plane_side side = plane.getSide(centre, halfSize);
                if (side == outside)
                {
                    // Found a splitting plane therefore return not intersecting
                    return false;
                }
            }

            // couldn't find a splitting plane, assume intersecting
            return true;

        }
        /** Intersection test with sphere<T>
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const sphere<T>& sphere) const
        {
            typename PlaneList::const_iterator i, iend;
            iend = planes.end();
            for (i = planes.begin(); i != iend; ++i)
            {
                const plane<T> & plane = *i;

                // Test which side of the plane the sphere is
                float d = plane.getDistance(sphere.getCenter());
                // Negate d if planes point inwards
                if (outside == NEGATIVE_SIDE) d = -d;

                if ( (d - sphere.getRadius()) > 0)
                    return false;
            }

            return true;

        }

        /** Intersection test with a ray<T>
        @returns std::pair of hit (bool) and distance
        @remarks May return false positives but will never miss an intersection.
        */
        inline std::pair<bool, float> intersects(const ray<T>& ray)
        {
            return math<T>::intersects(ray, planes, outside == plane<T> ::POSITIVE_SIDE);
        }

    };

	//template<typename T>
    //typedef std::vector<plane_bounded_volume<T> > plane_bounded_volume_list;
}

#endif

