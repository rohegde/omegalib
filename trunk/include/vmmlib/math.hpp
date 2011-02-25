#ifndef __VMML__MATH__HPP__
#define __VMML__MATH__HPP__

#include <cmath>
#include <vmmlib/vector.hpp>
#include <vmmlib/matrix.hpp>
#include <vmmlib/quaternion.hpp>
#include <vmmlib/plane.hpp>

namespace vmml
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! wrapper to enable array use where arrays would not be allowed otherwise
	template< class T, size_t d >
	struct array_wrapper
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

	template<typename T> class ray;
	template<typename T> class plane;
	template<typename T> class sphere;
	template<typename T> class axis_aligned_box;

	template<typename T> class rect: public array_wrapper<vector<2, T>, 2> {};

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
	template<typename T>
    class math 
    {
    protected:
        /// Size of the trig tables as determined by constructor.
        static int mTrigTableSize;

        /// Radian -> index factor value ( mTrigTableSize / 2 * Pi )
        static T mTrigTableFactor;
        static T* mSinTable;
        static T* mTanTable;

        /** Private function to build trig tables.
        */
        void inline buildTrigTables();

		static inline T SinTable (T fValue);
		static inline T TanTable (T fValue);
    public:
        /** Default constructor.
            @param
                trigTableSize Optional parameter to set the size of the
                tables used to implement Sin, Cos, Tan
        */
        inline math(unsigned int trigTableSize = 4096);

        /** Default destructor.
        */
        inline ~math();

		static inline int iabs (int iValue) { return ( iValue >= 0 ? iValue : -iValue ); }
		static inline int iceil (T fValue) { return int(std::ceil(fValue)); }
		static inline int ifloor (T fValue) { return int(std::floor(fValue)); }
        static int isign (int iValue);

		static inline T abs (T fValue) { return T(fabs(fValue)); }
		static inline T acos (T fValue);
		static inline T asin (T fValue);
		static inline T atan (T fValue) { return std::atan(fValue); }
		static inline T atan2 (T fY, T fX) { return std::atan2(fY,fX); }
		static inline T ceil (T fValue) { return T(std::ceil(fValue)); }
		static inline bool isNaN(T f)
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
        static inline T cos (T fValue, bool useTables = false) {
			return (!useTables) ? T(std::cos(fValue)) : SinTable(fValue + HalfPi);
		}

		static inline T exp (T fValue) { return T(std::exp(fValue)); }

		static inline T floor (T fValue) { return T(std::floor(fValue)); }

		static inline T log (T fValue) { return T(std::log(fValue)); }

		/// Stored value of log(2) for frequent use
		static const T Log2Base;

		static inline T log2 (T fValue) { return T(log(fValue)/Log2Base); }

		static inline T logN (T base, T fValue) { return T(log(fValue)/log(base)); }

		static inline T pow (T fBase, T fExponent) { return T(std::pow(fBase,fExponent)); }

        static T sign (T fValue);

        /** Sine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
        static inline T sin (T fValue, bool useTables = false) {
			return (!useTables) ? T(std::sin(fValue)) : SinTable(fValue);
		}

		static inline T sqr (T fValue) { return fValue*fValue; }

		static inline T sqrt (T fValue) { return T(std::sqrt(fValue)); }

        static inline T unitRandom ();  // in [0,1]

        static inline T rangeRandom (T fLow, T fHigh);  // in [fLow,fHigh]

        static inline T symmetricRandom ();  // in [-1,1]

        /** Tangent function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation - faster but less accurate.
        */
		static inline T tan (T fValue, bool useTables = false) {
			return (!useTables) ? T(std::tan(fValue)) : TanTable(fValue);
		}

		static inline T degreesToRadians(T degrees) { return degrees * DegToRad; }
        static inline T radiansToDegrees(T radians) { return radians * RadToDeg; }

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
        static inline bool pointInTri2D(const vector<2,T>& p, const vector<2,T>& a, 
			const vector<2,T>& b, const vector<2,T>& c);

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
        static inline bool pointInTri3D(const vector<3,T>& p, const vector<3,T>& a, 
			const vector<3,T>& b, const vector<3,T>& c, const vector<3,T>& normal);
        /** ray<T> / plane intersection, returns boolean result and distance. */
        static inline std::pair<bool, T> intersects(const ray<T>& ray, const plane<T>& plane);

        /** ray<T> / sphere intersection, returns boolean result and distance. */
        static inline std::pair<bool, T> intersects(const ray<T>& ray, const sphere<T>& sphere, 
            bool discardInside = true);
        
        /** ray<T> / box intersection, returns boolean result and distance. */
        static inline std::pair<bool, T> intersects(const ray<T>& ray, const axis_aligned_box<T>& box);

        /** ray<T> / box intersection, returns boolean result and two intersection distance.
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
        static inline bool intersects(const ray<T>& ray, const axis_aligned_box<T>& box,
            T* d1, T* d2);

        /** ray<T> / triangle intersection, returns boolean result and distance.
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
        static inline std::pair<bool, T> intersects(const ray<T>& ray, const vector<3,T>& a,
            const vector<3,T>& b, const vector<3,T>& c, const vector<3,T>& normal,
            bool positiveSide = true, bool negativeSide = true);

        /** ray<T> / triangle intersection, returns boolean result and distance.
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
        static inline std::pair<bool, T> intersects(const ray<T>& ray, const vector<3,T>& a,
            const vector<3,T>& b, const vector<3,T>& c,
            bool positiveSide = true, bool negativeSide = true);

        /** sphere<T> / box intersection test. */
        static inline bool intersects(const sphere<T>& sphere, const axis_aligned_box<T>& box);

        /** plane<T> / box intersection test. */
        static inline bool intersects(const plane<T>& plane, const axis_aligned_box<T>& box);

        /** ray<T> / convex plane list intersection test. 
        @param ray The ray to test with
        @param plaeList List of planes which form a convex volume
        @param normalIsOutside Does the normal point outside the volume
        */
        static inline std::pair<bool, T> intersects(
            const ray<T>& ray, const std::vector<plane<T> >& planeList, 
            bool normalIsOutside);
        /** ray<T> / convex plane list intersection test. 
        @param ray The ray to test with
        @param plaeList List of planes which form a convex volume
        @param normalIsOutside Does the normal point outside the volume
        */
        static inline std::pair<bool, T> intersects(
            const ray<T>& ray, const std::list<plane<T> >& planeList, 
            bool normalIsOutside);

        /** sphere<T> / plane intersection test. 
        @remarks NB just do a plane.getDistance(sphere.getCenter()) for more detail!
        */
        static inline bool intersects(const sphere<T>& sphere, const plane<T>& plane);

        /** Compare 2 reals, using tolerance for inaccuracies.
        */
        static inline bool floatEqual(T a, T b,
            T tolerance = std::numeric_limits<T>::epsilon());

        /** Calculates the tangent space vector for a given set of positions / texture coords. */
        static inline vector<3,T> calculateTangentSpaceVector(
            const vector<3,T>& position1, const vector<3,T>& position2, const vector<3,T>& position3,
            T u1, T v1, T u2, T v2, T u3, T v3);

        /** Build a reflection matrix for the passed in plane. */
        static inline matrix<4,4,T> buildReflectionMatrix(const plane<T>& p);
        /** Calculate a face normal, including the w component which is the offset from the origin. */
        static inline vector<4,T> calculateFaceNormal(const vector<3,T>& v1, const vector<3,T>& v2, const vector<3,T>& v3);
        /** Calculate a face normal, no w-information. */
        static inline vector<3,T> calculateBasicFaceNormal(const vector<3,T>& v1, const vector<3,T>& v2, const vector<3,T>& v3);
        /** Calculate a face normal without normalize, including the w component which is the offset from the origin. */
        static inline vector<4,T> calculateFaceNormalWithoutNormalize(const vector<3,T>& v1, const vector<3,T>& v2, const vector<3,T>& v3);
        /** Calculate a face normal without normalize, no w-information. */
        static inline vector<3,T> calculateBasicFaceNormalWithoutNormalize(const vector<3,T>& v1, const vector<3,T>& v2, const vector<3,T>& v3);

		/** Generates a value based on the Gaussian (normal) distribution function
			with the given offset and scale parameters.
		*/
		static inline T gaussianDistribution(T x, T offset = 0.0f, T scale = 1.0f);

		/** Clamp a value within an inclusive range. */
		static inline T Clamp(T val, T minval, T maxval)
		{
			assert (minval < maxval && "Invalid clamp range");
			return std::max(std::min(val, maxval), minval);
		}

		static inline matrix<4,4,T> makeViewMatrix(const vector<3,T>& position, const quaternion<T>& orientation);
		static inline matrix<4,4,T> makePerspectiveMatrix(float fov, float aspect, float nearZ, float farZ);

		/** Get a bounding radius value from a bounding box. */
		static inline T boundingRadiusFromAABB(const axis_aligned_box<T>& aabb);

		/** Compute a quaternion rotation transforming vector a to vector b **/
		static inline quaternion<T> buildRotation(const vector<3,T>& a, const vector<3,T>& b,
			const vector<3,T>& fallbackAxis = vector<3,T>::ZERO);

		static inline 
		ray<T> unproject(const vector<2, float>& pos, const matrix<4, 4, T>& modelview, const matrix<4, 4, T>& projection, const rect<int>& viewport, float z = 0.0f);

        static const T PositiveInfinity;
        static const T NegativeInfinity;
        static const T Pi;
        static const T TwoPi;
        static const T HalfPi;
		static const T DegToRad;
		static const T RadToDeg;
    };

    template<typename T>
    const T math<T>::PositiveInfinity = std::numeric_limits<T>::infinity();
    template<typename T>
    const T math<T>::NegativeInfinity = -std::numeric_limits<T>::infinity();
    template<typename T>
    const T math<T>::Pi = T( 4.0 * atan( 1.0 ) );
    template<typename T>
    const T math<T>::TwoPi = T( 2.0 * Pi );
    template<typename T>
    const T math<T>::HalfPi = T( 0.5 * Pi );
    template<typename T>
	const T math<T>::DegToRad = Pi / T(180.0);
    template<typename T>
	const T math<T>::RadToDeg = T(180.0) / Pi;
    template<typename T>
	const T math<T>::Log2Base = log(T(2.0));

    template<typename T>
    int math<T>::mTrigTableSize;

    template<typename T>
    T  math<T>::mTrigTableFactor;
    template<typename T>
    T *math<T>::mSinTable = NULL;
    template<typename T>
    T *math<T>::mTanTable = NULL;

    //-----------------------------------------------------------------------
    template<typename T> inline 
	math<T>::math( unsigned int trigTableSize )
    {
        mTrigTableSize = trigTableSize;
        mTrigTableFactor = mTrigTableSize / math<T>::TwoPi;

        mSinTable = new T[mTrigTableSize];
        mTanTable = new T[mTrigTableSize];

        buildTrigTables();
    }

    //-----------------------------------------------------------------------
    template<typename T> inline 
    math<T>::~math()
    {
		delete[] mSinTable;
		delete[] mTanTable;
		mSinTable = NULL;
		mTanTable = NULL;
    }

    //-----------------------------------------------------------------------
    template<typename T> inline 
    void math<T>::buildTrigTables(void)
    {
        // Build trig lookup tables
        // Could get away with building only Pi sized Sin table but simpler this 
        // way. Who cares, it'll ony use an extra 8k of memory anyway and I like 
        // simplicity.
        T angle;
        for (int i = 0; i < mTrigTableSize; ++i)
        {
            angle = math<T>::TwoPi * i / mTrigTableSize;
            mSinTable[i] = sin(angle);
            mTanTable[i] = tan(angle);
        }
    }
	//-----------------------------------------------------------------------	
    template<typename T> inline 
	T math<T>::SinTable (T fValue)
    {
        // Convert range to index values, wrap if required
        int idx;
        if (fValue >= 0)
        {
            idx = int(fValue * mTrigTableFactor) % mTrigTableSize;
        }
        else
        {
            idx = mTrigTableSize - (int(-fValue * mTrigTableFactor) % mTrigTableSize) - 1;
        }

        return mSinTable[idx];
    }
	//-----------------------------------------------------------------------
    template<typename T> inline 
	T math<T>::TanTable (T fValue)
    {
        // Convert range to index values, wrap if required
		int idx = int(fValue *= mTrigTableFactor) % mTrigTableSize;
		return mTanTable[idx];
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    int math<T>::isign (int iValue)
    {
        return ( iValue > 0 ? +1 : ( iValue < 0 ? -1 : 0 ) );
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    T math<T>::acos (T fValue)
    {
        if ( -1.0 < fValue )
        {
            if ( fValue < 1.0 )
                return acos(fValue);
            else
                return 0.0f;
        }
        else
        {
            return Pi;
        }
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    T math<T>::asin (T fValue)
    {
        if ( -1.0 < fValue )
        {
            if ( fValue < 1.0 )
                return asin(fValue);
            else
                return HalfPi;
        }
        else
        {
            return -HalfPi;
        }
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    T math<T>::sign (T fValue)
    {
        if ( fValue > 0.0 )
            return 1.0;

        if ( fValue < 0.0 )
            return -1.0;

        return 0.0;
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    T math<T>::unitRandom ()
	{
		return ((T)rand() / RAND_MAX);
	}    
    //-----------------------------------------------------------------------
    template<typename T> inline 
    T math<T>::rangeRandom (T fLow, T fHigh)
    {
        return (fHigh-fLow)*unitRandom() + fLow;
    }

    //-----------------------------------------------------------------------
    template<typename T> inline 
    T math<T>::symmetricRandom ()
    {
		return 2.0f * unitRandom() - 1.0f;
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
	bool math<T>::pointInTri2D(const vector<2,T>& p, const vector<2,T>& a, 
		const vector<2,T>& b, const vector<2,T>& c)
    {
		// Winding must be consistent from all edges for point to be inside
		vector<2,T> v1, v2;
		T dot[3];
		bool zeroDot[3];

		v1 = b - a;
		v2 = p - a;

		// Note we don't care about normalisation here since sign is all we need
		// It means we don't have to worry about magnitude of cross products either
		dot[0] = v1.cross(v2);
		zeroDot[0] = math<T>::floatEqual(dot[0], 0.0f, 1e-3f);


		v1 = c - b;
		v2 = p - b;

		dot[1] = v1.cross(v2);
		zeroDot[1] = math<T>::floatEqual(dot[1], 0.0f, 1e-3f);

		// Compare signs (ignore colinear / coincident points)
		if(!zeroDot[0] && !zeroDot[1] 
		&& math<T>::sign(dot[0]) != math<T>::sign(dot[1]))
		{
			return false;
		}

		v1 = a - c;
		v2 = p - c;

		dot[2] = v1.cross(v2);
		zeroDot[2] = math<T>::floatEqual(dot[2], 0.0f, 1e-3f);
		// Compare signs (ignore colinear / coincident points)
		if((!zeroDot[0] && !zeroDot[2] 
			&& math<T>::sign(dot[0]) != math<T>::sign(dot[2])) ||
			(!zeroDot[1] && !zeroDot[2] 
			&& math<T>::sign(dot[1]) != math<T>::sign(dot[2])))
		{
			return false;
		}


		return true;
    }
	//-----------------------------------------------------------------------
    template<typename T> inline 
	bool math<T>::pointInTri3D(const vector<3,T>& p, const vector<3,T>& a, 
		const vector<3,T>& b, const vector<3,T>& c, const vector<3,T>& normal)
	{
        // Winding must be consistent from all edges for point to be inside
		vector<3,T> v1, v2;
		T dot[3];
		bool zeroDot[3];

        v1 = b - a;
        v2 = p - a;

		// Note we don't care about normalisation here since sign is all we need
		// It means we don't have to worry about magnitude of cross products either
        dot[0] = v1.cross(v2).dot(normal);
		zeroDot[0] = math<T>::floatEqual(dot[0], 0.0f, 1e-3f);


        v1 = c - b;
        v2 = p - b;

		dot[1] = v1.cross(v2).dot(normal);
		zeroDot[1] = math<T>::floatEqual(dot[1], 0.0f, 1e-3f);

		// Compare signs (ignore colinear / coincident points)
		if(!zeroDot[0] && !zeroDot[1] 
			&& math<T>::sign(dot[0]) != math<T>::sign(dot[1]))
		{
            return false;
		}

        v1 = a - c;
        v2 = p - c;

		dot[2] = v1.cross(v2).dot(normal);
		zeroDot[2] = math<T>::floatEqual(dot[2], 0.0f, 1e-3f);
		// Compare signs (ignore colinear / coincident points)
		if((!zeroDot[0] && !zeroDot[2] 
			&& math<T>::sign(dot[0]) != math<T>::sign(dot[2])) ||
			(!zeroDot[1] && !zeroDot[2] 
			&& math<T>::sign(dot[1]) != math<T>::sign(dot[2])))
		{
			return false;
		}


        return true;
	}
    //-----------------------------------------------------------------------
    template<typename T> inline 
    bool math<T>::floatEqual( T a, T b, T tolerance )
    {
        if (fabs(b-a) <= tolerance)
            return true;
        else
            return false;
    }

    //-----------------------------------------------------------------------
    template<typename T> inline 
    std::pair<bool, T> math<T>::intersects(const ray<T>& ray, const plane<T>& plane)
    {

        T denom = plane.normal.dot(ray.getDirection());
        if (math<T>::abs(denom) < std::numeric_limits<T>::epsilon())
        {
            // Parallel
            return std::pair<bool, T>(false, 0);
        }
        else
        {
            T nom = plane.normal.dot(ray.getOrigin()) + plane.d;
            T t = -(nom/denom);
            return std::pair<bool, T>(t >= 0, t);
        }
        
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    std::pair<bool, T> math<T>::intersects(const ray<T>& ray, 
        const std::vector<plane<T> >& planes, bool normalIsOutside)
    {
		std::list<plane<T> > planesList;
		for (typename std::vector<plane<T> >::const_iterator i = planes.begin(); i != planes.end(); ++i)
		{
			planesList.push_back(*i);
		}
		return intersects(ray, planesList, normalIsOutside);
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    std::pair<bool, T> math<T>::intersects(const ray<T>& ray, 
        const std::list<plane<T> >& planes, bool normalIsOutside)
    {
		typename std::list<plane<T> >::const_iterator planeit, planeitend;
		planeitend = planes.end();
		bool allInside = true;
		std::pair<bool, T> ret;
		std::pair<bool, T> end;
		ret.first = false;
		ret.second = 0.0f;
		end.first = false;
		end.second = 0;


		// derive side
		// NB we don't pass directly since that would require plane<T>::Side in 
		// interface, which results in recursive includes since math is so fundamental
		typename plane<T>::Side outside = normalIsOutside ? plane<T>::POSITIVE_SIDE : plane<T>::NEGATIVE_SIDE;

		for (planeit = planes.begin(); planeit != planeitend; ++planeit)
		{
			const plane<T>& plane = *planeit;
			// is origin outside?
			if (plane.getSide(ray.getOrigin()) == outside)
			{
				allInside = false;
				// Test single plane
				std::pair<bool, T> planeRes = 
					ray.intersects(plane);
				if (planeRes.first)
				{
					// Ok, we intersected
					ret.first = true;
					// Use the most distant result since convex volume
					ret.second = std::max(ret.second, planeRes.second);
				}
				else
				{
					ret.first =false;
					ret.second=0.0f;
					return ret;
				}
			}
			else
			{
				std::pair<bool, T> planeRes = 
					ray.intersects(plane);
				if (planeRes.first)
				{
					if( !end.first )
					{
						end.first = true;
						end.second = planeRes.second;
					}
					else
					{
						end.second = std::min( planeRes.second, end.second );
					}

				}

			}
		}

		if (allInside)
		{
			// Intersecting at 0 distance since inside the volume!
			ret.first = true;
			ret.second = 0.0f;
			return ret;
		}

		if( end.first )
		{
			if( end.second < ret.second )
			{
				ret.first = false;
				return ret;
			}
		}
		return ret;
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    std::pair<bool, T> math<T>::intersects(const ray<T>& ray, const sphere<T>& sphere, 
        bool discardInside)
    {
        const vector<3,T>& raydir = ray.getDirection();
        // Adjust ray origin relative to sphere center
        const vector<3,T>& rayorig = ray.getOrigin() - sphere.getCenter();
        T radius = sphere.getRadius();

        // Check origin inside first
        if (rayorig.squaredLength() <= radius*radius && discardInside)
        {
            return std::pair<bool, T>(true, 0);
        }

        // Mmm, quadratics
        // Build coeffs which can be used with std quadratic solver
        // ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
        T a = raydir.dot(raydir);
        T b = 2 * rayorig.dot(raydir);
        T c = rayorig.dot(rayorig) - radius*radius;

        // Calc determinant
        T d = (b*b) - (4 * a * c);
        if (d < 0)
        {
            // No intersection
            return std::pair<bool, T>(false, 0);
        }
        else
        {
            // BTW, if d=0 there is one intersection, if d > 0 there are 2
            // But we only want the closest one, so that's ok, just use the 
            // '-' version of the solver
            T t = ( -b - math<T>::sqrt(d) ) / (2 * a);
            if (t < 0)
                t = ( -b + math<T>::sqrt(d) ) / (2 * a);
            return std::pair<bool, T>(true, t);
        }


    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
	std::pair<bool, T> math<T>::intersects(const ray<T>& ray, const axis_aligned_box<T>& box)
	{
		if (box.isNull()) return std::pair<bool, T>(false, 0);
		if (box.isInfinite()) return std::pair<bool, T>(true, 0);

		T lowt = 0.0f;
		T t;
		bool hit = false;
		vector<3,T> hitpoint;
		const vector<3,T>& min = box.getMinimum();
		const vector<3,T>& max = box.getMaximum();
		const vector<3,T>& rayorig = ray.getOrigin();
		const vector<3,T>& raydir = ray.getDirection();

		// Check origin inside first
		if ( rayorig > min && rayorig < max )
		{
			return std::pair<bool, T>(true, 0);
		}

		// Check each face in turn, only check closest 3
		// Min x
		if (rayorig.x() <= min.x() && raydir.x() > 0)
		{
			t = (min.x() - rayorig.x()) / raydir.x();
			if (t >= 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
					hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max x
		if (rayorig.x() >= max.x() && raydir.x() < 0)
		{
			t = (max.x() - rayorig.x()) / raydir.x();
			if (t >= 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
					hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Min y
		if (rayorig.y() <= min.y() && raydir.y() > 0)
		{
			t = (min.y() - rayorig.y()) / raydir.y();
			if (t >= 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
					hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max y
		if (rayorig.y() >= max.y() && raydir.y() < 0)
		{
			t = (max.y() - rayorig.y()) / raydir.y();
			if (t >= 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
					hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Min z
		if (rayorig.z() <= min.z() && raydir.z() > 0)
		{
			t = (min.z() - rayorig.z()) / raydir.z();
			if (t >= 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
					hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max z
		if (rayorig.z() >= max.z() && raydir.z() < 0)
		{
			t = (max.z() - rayorig.z()) / raydir.z();
			if (t >= 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
					hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}

		return std::pair<bool, T>(hit, lowt);

	} 
    //-----------------------------------------------------------------------
    template<typename T> inline 
    bool math<T>::intersects(const ray<T>& ray, const axis_aligned_box<T>& box,
        T* d1, T* d2)
    {
        if (box.isNull())
            return false;

        if (box.isInfinite())
        {
            if (d1) *d1 = 0;
            if (d2) *d2 = math<T>::PositiveInfinity;
            return true;
        }

        const vector<3,T>& min = box.getMinimum();
        const vector<3,T>& max = box.getMaximum();
        const vector<3,T>& rayorig = ray.getOrigin();
        const vector<3,T>& raydir = ray.getDirection();

        vector<3,T> absDir;
        absDir[0] = math<T>::abs(raydir[0]);
        absDir[1] = math<T>::abs(raydir[1]);
        absDir[2] = math<T>::abs(raydir[2]);

        // Sort the axis, ensure check minimise floating error axis first
        int imax = 0, imid = 1, imin = 2;
        if (absDir[0] < absDir[2])
        {
            imax = 2;
            imin = 0;
        }
        if (absDir[1] < absDir[imin])
        {
            imid = imin;
            imin = 1;
        }
        else if (absDir[1] > absDir[imax])
        {
            imid = imax;
            imax = 1;
        }

        T start = 0, end = math<T>::PositiveInfinity;

#define _CALC_AXIS(i)                                       \
    do {                                                    \
        T denom = 1 / raydir[i];                         \
        T newstart = (min[i] - rayorig[i]) * denom;      \
        T newend = (max[i] - rayorig[i]) * denom;        \
        if (newstart > newend) std::swap(newstart, newend); \
        if (newstart > end || newend < start) return false; \
        if (newstart > start) start = newstart;             \
        if (newend < end) end = newend;                     \
    } while(0)

        // Check each axis in turn

        _CALC_AXIS(imax);

        if (absDir[imid] < std::numeric_limits<T>::epsilon())
        {
            // Parallel with middle and minimise axis, check bounds only
            if (rayorig[imid] < min[imid] || rayorig[imid] > max[imid] ||
                rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
                return false;
        }
        else
        {
            _CALC_AXIS(imid);

            if (absDir[imin] < std::numeric_limits<T>::epsilon())
            {
                // Parallel with minimise axis, check bounds only
                if (rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
                    return false;
            }
            else
            {
                _CALC_AXIS(imin);
            }
        }
#undef _CALC_AXIS

        if (d1) *d1 = start;
        if (d2) *d2 = end;

        return true;
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    std::pair<bool, T> math<T>::intersects(const ray<T>& ray, const vector<3,T>& a,
        const vector<3,T>& b, const vector<3,T>& c, const vector<3,T>& normal,
        bool positiveSide, bool negativeSide)
    {
        //
        // Calculate intersection with plane.
        //
        T t;
        {
            T denom = normal.dot(ray.getDirection());

            // Check intersect side
            if (denom > + std::numeric_limits<T>::epsilon())
            {
                if (!negativeSide)
                    return std::pair<bool, T>(false, 0);
            }
            else if (denom < - std::numeric_limits<T>::epsilon())
            {
                if (!positiveSide)
                    return std::pair<bool, T>(false, 0);
            }
            else
            {
                // Parallel or triangle area is close to zero when
                // the plane normal not normalized.
                return std::pair<bool, T>(false, 0);
            }

            t = normal.dot(a - ray.getOrigin()) / denom;

            if (t < 0)
            {
                // Intersection is behind origin
                return std::pair<bool, T>(false, 0);
            }
        }

        //
        // Calculate the largest area projection plane in X, Y or Z.
        //
        size_t i0, i1;
        {
            T n0 = math<T>::abs(normal[0]);
            T n1 = math<T>::abs(normal[1]);
            T n2 = math<T>::abs(normal[2]);

            i0 = 1; i1 = 2;
            if (n1 > n2)
            {
                if (n1 > n0) i0 = 0;
            }
            else
            {
                if (n2 > n0) i1 = 0;
            }
        }

        //
        // Check the intersection point is inside the triangle.
        //
        {
            T u1 = b[i0] - a[i0];
            T v1 = b[i1] - a[i1];
            T u2 = c[i0] - a[i0];
            T v2 = c[i1] - a[i1];
            T u0 = t * ray.getDirection()[i0] + ray.getOrigin()[i0] - a[i0];
            T v0 = t * ray.getDirection()[i1] + ray.getOrigin()[i1] - a[i1];

            T alpha = u0 * v2 - u2 * v0;
            T beta  = u1 * v0 - u0 * v1;
            T area  = u1 * v2 - u2 * v1;

            // epsilon to avoid T precision error
            const T EPSILON = 1e-6f;

            T tolerance = - EPSILON * area;

            if (area > 0)
            {
                if (alpha < tolerance || beta < tolerance || alpha+beta > area-tolerance)
                    return std::pair<bool, T>(false, 0);
            }
            else
            {
                if (alpha > tolerance || beta > tolerance || alpha+beta < area-tolerance)
                    return std::pair<bool, T>(false, 0);
            }
        }

        return std::pair<bool, T>(true, t);
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    std::pair<bool, T> math<T>::intersects(const ray<T>& ray, const vector<3,T>& a,
        const vector<3,T>& b, const vector<3,T>& c,
        bool positiveSide, bool negativeSide)
    {
        vector<3,T> normal = calculateBasicFaceNormalWithoutNormalize(a, b, c);
        return intersects(ray, a, b, c, normal, positiveSide, negativeSide);
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    bool math<T>::intersects(const sphere<T>& sphere, const axis_aligned_box<T>& box)
    {
        if (box.isNull()) return false;
        if (box.isInfinite()) return true;

        // Use splitting planes
        const vector<3,T>& center = sphere.getCenter();
        T radius = sphere.getRadius();
        const vector<3,T>& min = box.getMinimum();
        const vector<3,T>& max = box.getMaximum();

		// Arvo's algorithm
		T s, d = 0;
		for (int i = 0; i < 3; ++i)
		{
			if (center[i] < min[i])
			{
				s = center[i] - min[i];
				d += s * s; 
			}
			else if(center[i] > max[i])
			{
				s = center[i] - max[i];
				d += s * s; 
			}
		}
		return d <= radius * radius;

    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    bool math<T>::intersects(const plane<T>& plane, const axis_aligned_box<T>& box)
    {
        return (plane.getSide(box) == BOTH_SIDE);
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    bool math<T>::intersects(const sphere<T>& sphere, const plane<T>& plane)
    {
        return (
            math<T>::abs(plane.getDistance(sphere.getCenter()))
            <= sphere.getRadius() );
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    vector<3,T> math<T>::calculateTangentSpaceVector(
        const vector<3,T>& position1, const vector<3,T>& position2, const vector<3,T>& position3,
        T u1, T v1, T u2, T v2, T u3, T v3)
    {
	    //side0 is the vector along one side of the triangle of vertices passed in, 
	    //and side1 is the vector along another side. Taking the cross product of these returns the normal.
	    vector<3,T> side0 = position1 - position2;
	    vector<3,T> side1 = position3 - position1;
	    //Calculate face normal
	    vector<3,T> normal = side1.cross(side0);
	    normal.normalize();
	    //Now we use a formula to calculate the tangent. 
	    T deltaV0 = v1 - v2;
	    T deltaV1 = v3 - v1;
	    vector<3,T> tangent = deltaV1 * side0 - deltaV0 * side1;
	    tangent.normalize();
	    //Calculate binormal
	    T deltaU0 = u1 - u2;
	    T deltaU1 = u3 - u1;
	    vector<3,T> binormal = deltaU1 * side0 - deltaU0 * side1;
	    binormal.normalize();
	    //Now, we take the cross product of the tangents to get a vector which 
	    //should point in the same direction as our normal calculated above. 
	    //If it points in the opposite direction (the dot product between the normals is less than zero), 
	    //then we need to reverse the s and t tangents. 
	    //This is because the triangle has been mirrored when going from tangent space to object space.
	    //reverse tangents if necessary
	    vector<3,T> tangentCross = tangent.cross(binormal);
	    if (tangentCross.dot(normal) < 0.0f)
	    {
		    tangent = -tangent;
		    binormal = -binormal;
	    }

        return tangent;

    }
    //-----------------------------------------------------------------------
    //matrix<4,4,T> math<T>::buildReflectionMatrix(const plane<T>& p)
    //{
    //    return matrix<4,4,T>(
    //        -2 * p.normal.x() * p.normal.x() + 1,   -2 * p.normal.x() * p.normal.y(),       -2 * p.normal.x() * p.normal.z(),       -2 * p.normal.x() * p.d, 
    //        -2 * p.normal.y() * p.normal.x(),       -2 * p.normal.y() * p.normal.y() + 1,   -2 * p.normal.y() * p.normal.z(),       -2 * p.normal.y() * p.d, 
    //        -2 * p.normal.z() * p.normal.x(),       -2 * p.normal.z() * p.normal.y(),       -2 * p.normal.z() * p.normal.z() + 1,   -2 * p.normal.z() * p.d, 
    //        0,                                  0,                                  0,                                  1);
    //}
    //-----------------------------------------------------------------------
    template<typename T> inline 
    vector<4,T> math<T>::calculateFaceNormal(const vector<3,T>& v1, const vector<3,T>& v2, const vector<3,T>& v3)
    {
        vector<3,T> normal = calculateBasicFaceNormal(v1, v2, v3);
        // Now set up the w (distance of tri from origin
        return vector<4,T>(normal.x(), normal.y(), normal.z(), -(normal.dot(v1)));
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    vector<3,T> math<T>::calculateBasicFaceNormal(const vector<3,T>& v1, const vector<3,T>& v2, const vector<3,T>& v3)
    {
        vector<3,T> normal = (v2 - v1).cross(v3 - v1);
        normal.normalize();
        return normal;
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    vector<4,T> math<T>::calculateFaceNormalWithoutNormalize(const vector<3,T>& v1, const vector<3,T>& v2, const vector<3,T>& v3)
    {
        vector<3,T> normal = calculateBasicFaceNormalWithoutNormalize(v1, v2, v3);
        // Now set up the w (distance of tri from origin)
        return vector<4,T>(normal.x(), normal.y(), normal.z(), -(normal.dot(v1)));
    }
    //-----------------------------------------------------------------------
    template<typename T> inline 
    vector<3,T> math<T>::calculateBasicFaceNormalWithoutNormalize(const vector<3,T>& v1, const vector<3,T>& v2, const vector<3,T>& v3)
    {
        vector<3,T> normal = (v2 - v1).cross(v3 - v1);
        return normal;
    }
	//-----------------------------------------------------------------------
    template<typename T> inline 
	T math<T>::gaussianDistribution(T x, T offset, T scale)
	{
		T nom = math<T>::exp(
			-math<T>::sqr(x - offset) / (2 * math<T>::sqr(scale)));
		T denom = scale * math<T>::sqrt(2 * math<T>::Pi);

		return nom / denom;

	}
	//---------------------------------------------------------------------
    template<typename T> inline 
	matrix<4,4,T> math<T>::makeViewMatrix(const vector<3,T>& position, const quaternion<T>& orientation)
	{
		matrix<4,4,T> viewMatrix;

		// View matrix is:
		//
		//  [ Lx  Uy  Dz  Tx  ]
		//  [ Lx  Uy  Dz  Ty  ]
		//  [ Lx  Uy  Dz  Tz  ]
		//  [ 0   0   0   1   ]
		//
		// Where T = -(Transposed(Rot) * Pos)

		// This is most efficiently done using 3x3 Matrices
		matrix<3,3,T> rot;
		orientation.get_rotation_matrix(rot);

		// Make the translation relative to new axes
		matrix<3,3,T> rotT;
		rot.transpose_to(rotT);
		vector<3,T> trans = -rotT * position;

		// Make final matrix
		viewMatrix = matrix<4,4,T>::IDENTITY;
		viewMatrix = rotT; // fills upper 3x3
		viewMatrix[0][3] = trans.x();
		viewMatrix[1][3] = trans.y();
		viewMatrix[2][3] = trans.z();
		viewMatrix[3][3] = 1.0f;

		return viewMatrix;

	}

	//---------------------------------------------------------------------
    template<typename T>
	inline matrix<4,4,T> math<T>::makePerspectiveMatrix(float fov, float a, float n, float f)
	{
		matrix<4,4,T> m = matrix<4,4,T>::ZERO;
		float e = 1.0f / tan(fov / 2);

		m[0][0] = e;
		m[1][1] = e / a;
		m[2][2] = - (f + n) / (f - n);
		m[2][3] = - (2 * f * n) / (f - n);
		m[3][2] = -1;

		return m;
	}

	//---------------------------------------------------------------------
    template<typename T> inline 
	T math<T>::boundingRadiusFromAABB(const axis_aligned_box<T>& aabb)
	{
		vector<3,T> max = aabb.getMaximum();
		vector<3,T> min = aabb.getMinimum();

		vector<3,T> magnitude = max;
		magnitude.makeCeil(-max);
		magnitude.makeCeil(min);
		magnitude.makeCeil(-min);

		return magnitude.length();
	}

		/** Gets the shortest arc quaternion to rotate this vector to the destination
		vector.
	@remarks
		If you call this with a dest vector that is close to the inverse
		of this vector, we will rotate 180 degrees around the 'fallbackAxis'
		(if specified, or a generated axis if not) since in this case
		ANY axis of rotation is valid.
	*/
    template<typename T> inline 
	quaternion<T> math<T>::buildRotation(const vector<3,T>& a, const vector<3,T>& b,
		const vector<3,T>& fallbackAxis)
	{
		// Based on Stan Melax's article in Game Programming Gems
		quaternion<T> q;
		// Copy, since cannot modify local
		vector<3,T> v0 = a;
		vector<3,T> v1 = b;
		v0.normalize();
		v1.normalize();

		T d = v0.dot(v1);
		// If dot == 1, vectors are the same
		if (d >= 1.0f)
		{
			return quaternion<T>::IDENTITY;
		}
		if (d < (1e-6f - 1.0f))
		{
			if (fallbackAxis != vector<3,T>::ZERO)
			{
				// rotate 180 degrees about the fallback axis
				q.fromAngleAxis(math<T>::Pi, fallbackAxis);
			}
			else
			{
				// Generate an axis
				vector<3,T> axis = vector<3,T>::UNIT_X.cross(a);
				if (axis.isZeroLength()) // pick another if colinear
					axis = vector<3,T>::UNIT_Y.cross(a);
				axis.normalize();
				q.fromAngleAxis(math<T>::Pi, axis);
			}
		}
		else
		{
			T s = math<T>::sqrt( (1+d)*2 );
			T invs = 1 / s;

			vector<3,T> c = v0.cross(v1);

    		q.x() = c.x() * invs;
			q.y() = c.y() * invs;
			q.z() = c.z() * invs;
			q.w() = s * 0.5f;
			q.normalize();
		}
		return q;
	}
	//---------------------------------------------------------------------
    template<typename T> inline 
	ray<T> math<T>::unproject(const vector<2, float>& point, const matrix<4, 4, T>& modelview, const matrix<4, 4, T>& projection, const rect<int>& viewport, float z)
	{
		vector<3, T> origin;
		vector<3, T> direction;

		matrix<4, 4, T> A = projection * modelview;
		matrix<4, 4, T> m;
		if(!A.inverse(m)) return ray<T>();

		vector<4, T> in;
		in[0]=(point[0] - (float)viewport[0][0]) / (float)(viewport[1][0]) * 2.0f - 1.0f;
        in[1]=((viewport[1][1] - point[1]) - (float)viewport[0][1]) / (float)(viewport[1][1]) * 2.0f - 1.0f;
        in[2]= -1.0f;
        in[3]=1.0f;

		vector<4, T> m1 = m * in;
		m1 = m1 / m1[3];

        in[2] = 1.0f;
		vector<4, T> m2 = m * in;
		m2 = m2 / m2[3];

		float t = (z - m1[2]) / (m2[2] - m1[2]);
		origin[0] = m1[0] + t * (m2[0] - m1[0]);
		origin[1] = m1[1] + t * (m2[1] - m1[1]);
		origin[2] = z;

		direction = vector<3, T>((m2[0] - m1[0]), (m2[1] - m1[1]), (m2[2] - m1[2]));
		direction.normalize();
		//direction *= -1;

		return ray<T>(origin, direction);
	}

} // namespace vmml

#endif

