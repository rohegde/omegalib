/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#include "oengine/MeshData.h"
#include "oengine/PlyDataReader.h"
//#include <cstdlib>
//#include <math.h>

#ifdef OMEGA_OS_LINUX
#include <float.h>
#endif

#ifdef OMEGA_OS_OSX
#include <cfloat>
#endif

using namespace omega;
using namespace oengine;

///////////////////////////////////////////////////////////////////////////////////////////////////
MeshData* MeshUtils::load(const String& file)
{
	if(StringUtils::endsWith(file, "ply"))
	{
		PlyDataReader* reader = new PlyDataReader();
		if(!reader->readPlyFile(file))
		{
			ofwarn("Could not load mesh file %1%.", %file);
		}
		else
		{
			return reader;
		}
	}
	else if(StringUtils::endsWith(file, "obj"))
	{
		// ObjDataReader* reader = new ObjDataReader();
		// if(!reader->readFile(meshFilename))
		// {
			// ofwarn("Could not load mesh file %1%.", %meshFilename);
		// }
		// else
		// {
			// reader->scale(0.8f);
			// ed->meshData = reader;
		// }
	}
	else
	{
		ofwarn("%1%: unsupported file format.", %file);
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshData::clear() 
{
	vertices.clear();
	colors.clear();
	normals.clear();
	triangles.clear();
	myHasColors = false;
	myHasNormals = false;
	myHasTexCoords0 = false;
	myHasTexCoords1 = false;
	myHasTexCoords2 = false;
	myHasTexCoords3 = false;
	myIsDirty = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void MeshData::update()
{
	// Reset bounding box.
	float fmax = std::numeric_limits<float>::max();
	float fmin = -std::numeric_limits<float>::max();
	Vector3f vmin(fmax, fmax, fmax);
	Vector3f vmax(fmin, fmin, fmin);

	// Recompute bounding box.
	for(int i = 0; i < getNumVertices(); i++)
	{
		const Vector3f& v = getVertexPosition(i);
		for(int j = 0; j < 3; j++)
		{
			if(v[j] < vmin[j]) vmin[j] = v[j];
			if(v[j] > vmax[j]) vmax[j] = v[j];
		}
	}

	myBBox.setExtents(vmin, vmax);

	myIsDirty = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshData::calculateNormals( const bool vertexNormals )
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

#define min(a, b)  ((a)<(b))?(a):(b)
#define max(a, b)  ((a)>(b))?(a):(b)

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshData::calculateBoundingBox()
{
	myBBox.setNull();
	
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
	myBBox.setExtents(vmin, vmax);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Axis MeshData::getLongestAxis( const size_t start,	const size_t elements ) const
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

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshData::scale( const float baseSize )
{
	// calculate bounding box if not yet done
	if(myBBox.isNull())
		calculateBoundingBox();
    
	// find largest dimension and determine scale factor
	float factor = 0.0f;
	for( size_t i = 0; i < 3; ++i )
		factor = max( factor, myBBox[1][i] - myBBox[0][i] );
	factor = baseSize / factor;
    
	// determine scale offset
	Vector3f offset;
	for( size_t i = 0; i < 3; ++i )
		offset[i] = ( myBBox[0][i] + myBBox[1][i] ) * 0.5f;
    
	// scale the data
	for( size_t v = 0; v < vertices.size(); ++v )
		for( size_t i = 0; i < 3; ++i )
		{
			vertices[v][i] -= offset[i];
			vertices[v][i] *= factor;
		}
    
	Vector3f min, max;
	min = (myBBox[0] - offset) * factor;
	max = (myBBox[1] - offset) * factor;

	// scale the bounding box
	myBBox.setExtents(min, max);
}
