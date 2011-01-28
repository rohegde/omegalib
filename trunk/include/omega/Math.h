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
#ifndef __Math_H__
#define __Math_H__

#include "osystem.h"

namespace omega
{
	// Forward declarations.
	class Sphere;
	class Ray;
	class AxisAlignedBox;
	class Plane;

    /** Class to provide access to common mathematical functions.
        @remarks
            Most of the maths functions are aliased versions of the C runtime
            library functions. They are aliased here to provide future
            optimisation opportunities, either from faster RTLs or custom
            math approximations.
        @note
            <br>This is based on MgcMath.h from
            <a href="http://www.geometrictools.com/">Wild Magic</a>.
    */
    class OMEGA_API Math 
    {
    protected:
        /// Size of the trig tables as determined by constructor.
        static int mTrigTableSize;

        /// Radian -> index factor value ( mTrigTableSize / 2 * Pi )
        static float mTrigTableFactor;
        static float* mSinTable;
        static float* mTanTable;

        /** Private function to build trig tables.
        */
        void buildTrigTables();

		static float SinTable (float fValue);
		static float TanTable (float fValue);
    public:
        /** Default constructor.
            @param
                trigTableSize Optional parameter to set the size of the
                tables used to implement Sin, Cos, Tan
        */
        Math(unsigned int trigTableSize = 4096);

        /** Default destructor.
        */
        ~Math();

		static inline int iabs (int iValue) { return ( iValue >= 0 ? iValue : -iValue ); }
		static inline int iceil (float fValue) { return int(std::ceil(fValue)); }
		static inline int ifloor (float fValue) { return int(std::floor(fValue)); }
        static int isign (int iValue);

		static inline float abs (float fValue) { return float(fabs(fValue)); }
		static float acos (float fValue);
		static float asin (float fValue);
		static inline float atan (float fValue) { return std::atan(fValue); }
		static inline float atan2 (float fY, float fX) { return std::atan2(fY,fX); }
		static inline float ceil (float fValue) { return float(std::ceil(fValue)); }
		static inline bool isNaN(float f)
		{
			// std::isnan() is C99, not supported by all compilers
			// However NaN always fails this next test, no other number does.
			return f != f;
		}

        /** Cosine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline float cos (float fValue, bool useTables = false) {
			return (!useTables) ? float(std::cos(fValue)) : SinTable(fValue + HalfPi);
		}

		static inline float exp (float fValue) { return float(std::exp(fValue)); }

		static inline float floor (float fValue) { return float(std::floor(fValue)); }

		static inline float log (float fValue) { return float(std::log(fValue)); }

		/// Stored value of log(2) for frequent use
		static const float Log2Base;

		static inline float log2 (float fValue) { return float(log(fValue)/Log2Base); }

		static inline float logN (float base, float fValue) { return float(log(fValue)/log(base)); }

		static inline float pow (float fBase, float fExponent) { return float(std::pow(fBase,fExponent)); }

        static float sign (float fValue);

        /** Sine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline float sin (float fValue, bool useTables = false) {
			return (!useTables) ? float(std::sin(fValue)) : SinTable(fValue);
		}

		static inline float sqr (float fValue) { return fValue*fValue; }

		static inline float sqrt (float fValue) { return float(std::sqrt(fValue)); }

        static float unitRandom ();  // in [0,1]

        static float rangeRandom (float fLow, float fHigh);  // in [fLow,fHigh]

        static float symmetricRandom ();  // in [-1,1]

        /** Tangent function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
		static inline float tan (float fValue, bool useTables = false) {
			return (!useTables) ? float(std::tan(fValue)) : TanTable(fValue);
		}

		static inline float degreesToRadians(float degrees) { return degrees * DegToRad; }
        static inline float radiansToDegrees(float radians) { return radians * RadToDeg; }

       /** Checks whether a given point is inside a triangle, in a
            2-dimensional (Cartesian) space.
            @remarks
                The vertices of the triangle must be given in either
                trigonometrical (anticlockwise) or inverse trigonometrical
                (clockwise) order.
            @param
                p The point.
            @param
                a The triangle's first vertex.
            @param
                b The triangle's second vertex.
            @param
                c The triangle's third vertex.
            @returns
                If the point resides in the triangle, <b>true</b> is
                returned.
            @par
                If the point is outside the triangle, <b>false</b> is
                returned.
        */
        static bool pointInTri2D(const Vector2f& p, const Vector2f& a, 
			const Vector2f& b, const Vector2f& c);

       /** Checks whether a given 3D point is inside a triangle.
       @remarks
            The vertices of the triangle must be given in either
            trigonometrical (anticlockwise) or inverse trigonometrical
            (clockwise) order, and the point must be guaranteed to be in the
			same plane as the triangle
        @param
            p The point.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
		@param 
			normal The triangle plane's normal (passed in rather than calculated
				on demand since the caller may already have it)
        @returns
            If the point resides in the triangle, <b>true</b> is
            returned.
        @par
            If the point is outside the triangle, <b>false</b> is
            returned.
        */
        static bool pointInTri3D(const Vector3f& p, const Vector3f& a, 
			const Vector3f& b, const Vector3f& c, const Vector3f& normal);
        /** Ray / plane intersection, returns boolean result and distance. */
        static std::pair<bool, float> intersects(const Ray& ray, const Plane& plane);

        /** Ray / sphere intersection, returns boolean result and distance. */
        static std::pair<bool, float> intersects(const Ray& ray, const Sphere& sphere, 
            bool discardInside = true);
        
        /** Ray / box intersection, returns boolean result and distance. */
        static std::pair<bool, float> intersects(const Ray& ray, const AxisAlignedBox& box);

        /** Ray / box intersection, returns boolean result and two intersection distance.
        @param
            ray The ray.
        @param
            box The box.
        @param
            d1 A real pointer to retrieve the near intersection distance
                from the ray origin, maybe <b>null</b> which means don't care
                about the near intersection distance.
        @param
            d2 A real pointer to retrieve the far intersection distance
                from the ray origin, maybe <b>null</b> which means don't care
                about the far intersection distance.
        @returns
            If the ray is intersects the box, <b>true</b> is returned, and
            the near intersection distance is return by <i>d1</i>, the
            far intersection distance is return by <i>d2</i>. Guarantee
            <b>0</b> <= <i>d1</i> <= <i>d2</i>.
        @par
            If the ray isn't intersects the box, <b>false</b> is returned, and
            <i>d1</i> and <i>d2</i> is unmodified.
        */
        static bool intersects(const Ray& ray, const AxisAlignedBox& box,
            float* d1, float* d2);

        /** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
		@param 
			normal The triangle plane's normal (passed in rather than calculated
				on demand since the caller may already have it), doesn't need
                normalised since we don't care.
        @param
            positiveSide Intersect with "positive side" of the triangle
        @param
            negativeSide Intersect with "negative side" of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> returned.
        */
        static std::pair<bool, float> intersects(const Ray& ray, const Vector3f& a,
            const Vector3f& b, const Vector3f& c, const Vector3f& normal,
            bool positiveSide = true, bool negativeSide = true);

        /** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
        @param
            positiveSide Intersect with "positive side" of the triangle
        @param
            negativeSide Intersect with "negative side" of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> returned.
        */
        static std::pair<bool, float> intersects(const Ray& ray, const Vector3f& a,
            const Vector3f& b, const Vector3f& c,
            bool positiveSide = true, bool negativeSide = true);

        /** Sphere / box intersection test. */
        static bool intersects(const Sphere& sphere, const AxisAlignedBox& box);

        /** Plane / box intersection test. */
        static bool intersects(const Plane& plane, const AxisAlignedBox& box);

        /** Ray / convex plane list intersection test. 
        @param ray The ray to test with
        @param plaeList List of planes which form a convex volume
        @param normalIsOutside Does the normal point outside the volume
        */
        static std::pair<bool, float> intersects(
            const Ray& ray, const std::vector<Plane>& planeList, 
            bool normalIsOutside);
        /** Ray / convex plane list intersection test. 
        @param ray The ray to test with
        @param plaeList List of planes which form a convex volume
        @param normalIsOutside Does the normal point outside the volume
        */
        static std::pair<bool, float> intersects(
            const Ray& ray, const std::list<Plane>& planeList, 
            bool normalIsOutside);

        /** Sphere / plane intersection test. 
        @remarks NB just do a plane.getDistance(sphere.getCenter()) for more detail!
        */
        static bool intersects(const Sphere& sphere, const Plane& plane);

        /** Compare 2 reals, using tolerance for inaccuracies.
        */
        static bool floatEqual(float a, float b,
            float tolerance = std::numeric_limits<float>::epsilon());

        /** Calculates the tangent space vector for a given set of positions / texture coords. */
        static Vector3f calculateTangentSpaceVector(
            const Vector3f& position1, const Vector3f& position2, const Vector3f& position3,
            float u1, float v1, float u2, float v2, float u3, float v3);

        /** Build a reflection matrix for the passed in plane. */
        static Matrix4f buildReflectionMatrix(const Plane& p);
        /** Calculate a face normal, including the w component which is the offset from the origin. */
        static Vector4f calculateFaceNormal(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3);
        /** Calculate a face normal, no w-information. */
        static Vector3f calculateBasicFaceNormal(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3);
        /** Calculate a face normal without normalize, including the w component which is the offset from the origin. */
        static Vector4f calculateFaceNormalWithoutNormalize(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3);
        /** Calculate a face normal without normalize, no w-information. */
        static Vector3f calculateBasicFaceNormalWithoutNormalize(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3);

		/** Generates a value based on the Gaussian (normal) distribution function
			with the given offset and scale parameters.
		*/
		static float gaussianDistribution(float x, float offset = 0.0f, float scale = 1.0f);

		/** Clamp a value within an inclusive range. */
		template <typename T>
		static T Clamp(T val, T minval, T maxval)
		{
			assert (minval < maxval && "Invalid clamp range");
			return std::max(std::min(val, maxval), minval);
		}

		static Matrix4f makeViewMatrix(const Vector3f& position, const Quaternion& orientation, 
			const Matrix4f* reflectMatrix = 0);

		/** Get a bounding radius value from a bounding box. */
		static float boundingRadiusFromAABB(const AxisAlignedBox& aabb);

		/** Compute a quaternion rotation transforming vector a to vector b **/
		static Quaternion buildRotation(const Vector3f& a, const Vector3f& b,
			const Vector3f& fallbackAxis = Vector3f::ZERO);


        static const float PositiveInfinity;
        static const float NegativeInfinity;
        static const float Pi;
        static const float TwoPi;
        static const float HalfPi;
		static const float DegToRad;
		static const float RadToDeg;

    };
}
#endif
