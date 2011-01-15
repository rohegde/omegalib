/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *********************************************************************************************************************/
#ifndef __OTYPES_H__
#define __OTYPES_H__

typedef unsigned char byte;
typedef unsigned int uint;

namespace omega
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
class Application;

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
// Type definitions.
typedef vmml::matrix< 3, 3, double > Matrix3d; //!< A 3x3 double matrix
typedef vmml::matrix< 4, 4, double > Matrix4d; //!< A 4x4 double matrix
typedef vmml::matrix< 3, 3, float >  Matrix3f; //!< A 3x3 float matrix
typedef vmml::matrix< 4, 4, float >  Matrix4f; //!< A 4x4 float matrix
typedef vmml::vector< 2, int > Vector2i; //!< A two-component integer vector
typedef vmml::vector< 3, int > Vector3i; //!< A three-component integer vector
typedef vmml::vector< 4, int > Vector4i; //!< A four-component integer vector
typedef vmml::vector< 3, double >Vector3d; //!< A three-component double vector
typedef vmml::vector< 4, double >Vector4d; //!< A four-component double vector
typedef vmml::vector< 2, float > Vector2f; //!< A two-component float vector
typedef vmml::vector< 3, float > Vector3f; //!< A three-component float vector
typedef vmml::vector< 4, float > Vector4f; //!< A four-component float vector
typedef ArrayWrapper< Vector3f, 2 >   BoundingBox;
typedef vmml::vector< 4, float >    BoundingSphere;
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

// Setting
typedef libconfig::Setting Setting;
typedef co::base::uint128_t uint128_t;

}; // namespace omega

#endif