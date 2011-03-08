/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of Eyescale Software GmbH nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *-------------------------------------------------------------------------------------------------
 * This file is a modified version of Vector3fData, eqPly, Equalizer.
 * Copyright (c) 2007, Tobias Wolf <twolf@access.unizh.ch>
 *               2009, Stefan Eilemann <eile@equalizergraphics.com>
 *************************************************************************************************/
#include "omega/scene/PlyDataReader.h"
#include "omega/StringUtils.h"
#include <cstdlib>
#include <algorithm>
#include <math.h>

#ifdef OMEGA_OS_LINUX
#include <float.h>
#endif

#ifdef OMEGA_OS_OSX
#include <cfloat>
#endif

#include "omega/glheaders.h"

#define min(a, b)  ((a)<(b))?(a):(b)
#define max(a, b)  ((a)>(b))?(a):(b)

typedef unsigned char uint8_t;

using namespace omega;
using namespace omega::scene;


/*  Contructor.  */
PlyDataReader::PlyDataReader()
    : _invertFaces( false )
{
	_boundingBox.setNull();
}


/*  Read the vertex and (if available/wanted) color data from the open file.  */
void PlyDataReader::readVertices( PlyFile* file, const int nVertices, 
	                               const bool readColors )
{
    // temporary vertex structure for ply loading
    struct _Vertex
    {
        float           x;
        float           y;
        float           z;
        unsigned char   r;
        unsigned char   g;
        unsigned char   b;
    } vertex;

    PlyProperty vertexProps[] = 
    {
        { "x", PLY_FLOAT, PLY_FLOAT, offsetof( _Vertex, x ), 0, 0, 0, 0 },
        { "y", PLY_FLOAT, PLY_FLOAT, offsetof( _Vertex, y ), 0, 0, 0, 0 },
        { "z", PLY_FLOAT, PLY_FLOAT, offsetof( _Vertex, z ), 0, 0, 0, 0 },
        { "red", PLY_UCHAR, PLY_UCHAR, offsetof( _Vertex, r ), 0, 0, 0, 0 },
        { "green", PLY_UCHAR, PLY_UCHAR, offsetof( _Vertex, g ), 0, 0, 0, 0 },
        { "blue", PLY_UCHAR, PLY_UCHAR, offsetof( _Vertex, b ), 0, 0, 0, 0 }
    };
    
    // use all 6 properties when reading colors, only the first 3 otherwise
    int limit = readColors ? 6 : 3;
    for( int i = 0; i < limit; ++i ) 
        ply_get_property( file, "vertex", &vertexProps[i] );
    
    vertices.clear();
    vertices.reserve( nVertices );
    
    if( readColors )
    {
        colors.clear();
        colors.reserve( nVertices );
    }
    
    // read in the vertices
    for( int i = 0; i < nVertices; ++i )
    {
        ply_get_element( file, static_cast< void* >( &vertex ) );
        vertices.push_back( Vector3f( vertex.x, vertex.y, vertex.z ) );
        if( readColors )
            colors.push_back( Color( (float)vertex.r / 255, (float)vertex.g  / 255, (float)vertex.b  / 255,  1.0f) );
    }
}


/*  Read the index data from the open file.  */
void PlyDataReader::readTriangles( PlyFile* file, const int nFaces )
{
    // temporary face structure for ply loading
    struct _Face
    {
        unsigned char   nVertices;
        int*            vertices;
    } face;

    PlyProperty faceProps[] = 
    {
        { "vertex_indices", PLY_INT, PLY_INT, offsetof( _Face, vertices ), 
          1, PLY_UCHAR, PLY_UCHAR, offsetof( _Face, nVertices ) }
    };
    
    ply_get_property( file, "face", &faceProps[0] );
    
    triangles.clear();
    triangles.reserve( nFaces );
    
    // read in the faces, asserting that they are only triangles
    uint8_t ind1 = _invertFaces ? 2 : 0;
    uint8_t ind3 = _invertFaces ? 0 : 2;
    for( int i = 0; i < nFaces; ++i )
    {
        ply_get_element( file, static_cast< void* >( &face ) );
        oassert( face.vertices != 0 );
        if( face.nVertices != 3 )
        {
            free( face.vertices );
            oerror( "Error reading PLY file. Encountered a face which does not have three vertices." );
        }
        triangles.push_back( Triangle( face.vertices[ind1], 
                                       face.vertices[1],
                                       face.vertices[ind3] ) );
        
        // free the memory that was allocated by ply_get_element
        free( face.vertices );
    }
}


/*  Open a PLY file and read vertex, color and index data.  */
bool PlyDataReader::readPlyFile( const std::string& filename )
{
    int     nPlyElems;
    char**  elemNames;
    int     fileType;
    float   version;
    bool    result = false;
    
    PlyFile* file = ply_open_for_reading( const_cast<char*>( filename.c_str( )),
                                          &nPlyElems, &elemNames, 
                                          &fileType, &version );
    if( !file )
    {
        oferror("Unable to open PLY file %1% for reading", %filename);
        return result;
    }
    oassert( elemNames != 0 );
    
    ofmsg("%1%: %1% elements, file type = %3% %4%", %filename %nPlyElems %fileType %version); 
    
    for( int i = 0; i < nPlyElems; ++i )
    {
        int nElems;
        int nProps;
        
        PlyProperty** props = ply_get_element_description( file, elemNames[i], 
                                                           &nElems, &nProps );
        oassert( props != 0 );
        
        if( equal_strings( elemNames[i], "vertex" ) )
        {
            bool hasColors = false;
            // determine if the file stores vertex colors
            for( int j = 0; j < nProps; ++j )
                if( equal_strings( props[j]->name, "red" ) )
                    hasColors = true;
            
            readVertices( file, nElems, hasColors );
            oassert( vertices.size() == static_cast< size_t >( nElems ) );
            if( hasColors )
            {
                oassert( colors.size() == static_cast< size_t >( nElems ));
            }
        }
        else if( equal_strings( elemNames[i], "face" ) )
        {
            readTriangles( file, nElems );
            oassert( triangles.size() == static_cast< size_t >( nElems ) );
            result = true;
        }
        
        // free the memory that was allocated by ply_get_element_description
        for( int j = 0; j < nProps; ++j )
            free( props[j] );
        free( props );
    }
    
    ply_close( file );
    
    // free the memory that was allocated by ply_open_for_reading
    for( int i = 0; i < nPlyElems; ++i )
        free( elemNames[i] );
    free( elemNames );

	if(this->normals.size() != 0) myHasNormals = true;
	else myHasNormals = false;

	if(this->colors.size() != 0) myHasColors = true;
	else myHasColors = false;

	this->myIsDirty = true;
    
    return result;
}


/*  Calculate the face or vertex normals of the current vertex data.  */
void PlyDataReader::calculateNormals( const bool vertexNormals )
{
    int wrongNormals = 0;
    
    normals.clear();
    if( vertexNormals )
    {
        normals.reserve( vertices.size() );
        
        // initialize all normals to zero
        for( size_t i = 0; i < vertices.size(); ++i )
            normals.push_back( Vector3f( 0, 0, 0 ) );
    }
    else
        normals.reserve( triangles.size() );
    
    // iterate over all triangles and add their normals to adjacent vertices
    Vector3f  triangleNormal;
    uint   i0, i1, i2;
    for( size_t i = 0; i < triangles.size(); ++i )
    {
        i0 = triangles[i](0);
        i1 = triangles[i](1);
        i2 = triangles[i](2);
        triangleNormal = Math::normal(  vertices[i0],
                                       vertices[i1],
                                       vertices[i2] );
        
        // count emtpy normals in debug mode
        if( triangleNormal.norm() == 0.0f ) ++wrongNormals;
         
        if( vertexNormals )
        {
            normals[i0] += triangleNormal; 
            normals[i1] += triangleNormal; 
            normals[i2] += triangleNormal;
        }
        else
            normals.push_back( triangleNormal ); 
    }
    
    // normalize all the normals
    if( vertexNormals )
        for( size_t i = 0; i < vertices.size(); ++i )
            normals[i].normalize();
    
    if( wrongNormals > 0 ) ofmsg("%1% faces had no valid normal.", %wrongNormals);
}


/*  Calculate the bounding box of the current vertex data.  */
void PlyDataReader::calculateBoundingBox()
{
	_boundingBox.setNull();
	
	float m = FLT_MAX;
	Vector3f vmin = Vector3f(m, m,  m);
	Vector3f vmax = Vector3f(-m, -m,  -m);

    for( size_t v = 1; v < vertices.size(); ++v )
	{
		Vector3f& ve = vertices[v];
		for(int i = 0; i < 3; i++)
		{
			vmin[i] = min(vmin[i], ve[i]);
			vmax[i] = max(vmax[i], ve[i]);
		}
	}
	_boundingBox.setExtents(vmin, vmax);
}


/* Calculates longest axis for a set of triangles */
Axis PlyDataReader::getLongestAxis( const size_t start,
                                 const size_t elements ) const
{
    if( start + elements > triangles.size() )
    {
		return AxisX;
    }

    AlignedBox3 bb;
    bb[0] = vertices[ triangles[start][0] ];
    bb[1] = vertices[ triangles[start][0] ];

    for( size_t t = start; t < start+elements; ++t )
        for( size_t v = 0; v < 3; ++v )
            for( size_t i = 0; i < 3; ++i )
            {
				bb[0][i] = min( bb[0][i], vertices[ triangles[t][v] ][i] );
                bb[1][i] = max( bb[1][i], vertices[ triangles[t][v] ][i] );
            }

    const GLfloat bbX = bb[1][0] - bb[0][0];
    const GLfloat bbY = bb[1][1] - bb[0][1];
    const GLfloat bbZ = bb[1][2] - bb[0][2];

    if( bbX >= bbY && bbX >= bbZ )
        return AxisY;

    if( bbY >= bbX && bbY >= bbZ )
        return AxisZ;

    return AxisZ;
}


/*  Scales the data to be within +- baseSize/2 (default 2.0) coordinates.  */
void PlyDataReader::scale( const float baseSize )
{
    // calculate bounding box if not yet done
	if(_boundingBox.isNull())
        calculateBoundingBox();
    
    // find largest dimension and determine scale factor
    float factor = 0.0f;
    for( size_t i = 0; i < 3; ++i )
        factor = max( factor, _boundingBox[1][i] - _boundingBox[0][i] );
    factor = baseSize / factor;
    
    // determine scale offset
    Vector3f offset;
    for( size_t i = 0; i < 3; ++i )
        offset[i] = ( _boundingBox[0][i] + _boundingBox[1][i] ) * 0.5f;
    
    // scale the data
    for( size_t v = 0; v < vertices.size(); ++v )
        for( size_t i = 0; i < 3; ++i )
        {
            vertices[v][i] -= offset[i];
            vertices[v][i] *= factor;
        }
    
	Vector3f min, max;
	min = (_boundingBox[0] - offset) * factor;
	max = (_boundingBox[1] - offset) * factor;

    // scale the bounding box
	_boundingBox.setExtents(min, max);
}


/** @cond IGNORE */
/*  Helper structure to sort Triangles with standard library sort function.  */
struct _TriangleSort
{
    _TriangleSort( PlyDataReader& data, const Axis axis ) : _data( data ),
                                                               _axis( axis ) {}
    
    bool operator() ( const Triangle& t1, const Triangle& t2 )
    {
        // references to both triangles' three vertices
        const Vector3f& v11 = _data.getVertexPosition( t1[0] );
        const Vector3f& v12 = _data.getVertexPosition( t1[1] );
        const Vector3f& v13 = _data.getVertexPosition( t1[2] );
        const Vector3f& v21 = _data.getVertexPosition( t2[0] );
        const Vector3f& v22 = _data.getVertexPosition( t2[1] );
        const Vector3f& v23 = _data.getVertexPosition( t2[2] );
        
        // compare first by given axis
        int axis = _axis;
        do
        {
            // test median of 'axis' component of all three vertices
            const float median1 = (v11[axis] + v12[axis] + v13[axis] ) / 3.0f;
            const float median2 = (v21[axis] + v22[axis] + v23[axis] ) / 3.0f;
            if( median1 != median2 )
                return ( median1 < median2 );
            
            // if still equal, move on to the next axis
            axis = ( axis + 1 ) % 3;
        }
        while( axis != _axis );
        
        return false;
    }
    
    PlyDataReader&   _data;
    const Axis          _axis;
};
/** @endcond */

/*  Sort the index data from start to start + length along the given axis.  */
void PlyDataReader::sort( const uint start, const uint length, const Axis axis )
{
    oassert( length > 0 );
    oassert( start + length <= triangles.size() );
    
    std::sort( triangles.begin() + start, 
               triangles.begin() + start + length,
               _TriangleSort( *this, axis ) );
}
