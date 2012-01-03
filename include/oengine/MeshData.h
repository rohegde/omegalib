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
#ifndef MESH_DATA
#define MESH_DATA

#include "oenginebase.h"

namespace oengine {
	///////////////////////////////////////////////////////////////////////////////////////////////
	//! Used to store mesh data loaded from a file in a common format, before transfering it to 
	//! vertex and index buffers and material specifications. Mesh readers either derive from this 
	//! class, or have a method that returns an instance of it.
    class OENGINE_API MeshData
    {
    public:
		MeshData() { clear(); }

		int getNumVertices();
		int getNumTriangles();
		
		Vector3f& getVertexPosition(int i);
		Color& getVertexColor(int i);
		Vector3f& getVertexNormal(int i);
		Triangle& getTriangle(int i);
		Vector2f& getTexCoords0(int i);
		Vector2f& getTexCoords1(int i);
		Vector2f& getTexCoords2(int i);
		Vector2f& getTexCoords3(int i);

		void addTriangle(const Triangle& tri); 
		void addVertex(const Vector3f& pos);
		void addVertex(const Vector3f& pos, const Vector3f& normal);
		void addVertex(const Vector3f& pos, const Vector3f& normal, const Color& color);
		void addVertex(const Vector3f& pos, const Color& color);
		bool hasColors();
		bool hasNormals();
		bool hasTexCoords0();
		bool hasTexCoords1();
		bool hasTexCoords2();
		bool hasTexCoords3();
		const AlignedBox3& getBoundingBox();
		void clear();
	
        //! Calculate the face or vertex normals of the current vertex data.
		void calculateNormals( const bool vertexNormals = true );
		//! Calculate the bounding box of the current vertex data.
        void calculateBoundingBox();
		//! Calculates longest axis for a set of triangles
        Axis getLongestAxis( const size_t start, const size_t elements ) const;
		//! Scales the data to be within +- baseSize/2 (default 2.0) coordinates.
		void scale( const float baseSize = 2.0f );

	protected:
        Vector< Vector3f >   vertices;
        Vector< Color >    colors;
        Vector< Vector3f >   normals;
        Vector< Triangle > triangles;
        Vector< Vector2f > texCoords0;
        Vector< Vector2f > texCoords1;
        Vector< Vector2f > texCoords2;
        Vector< Vector2f > texCoords3;

		bool myHasColors;
		bool myHasNormals;
		bool myHasTexCoords0;
		bool myHasTexCoords1;
		bool myHasTexCoords2;
		bool myHasTexCoords3;

		bool myIsDirty;
        AlignedBox3 myBBox;

	private:
		void update();
    };

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OENGINE_API MeshUtils
	{
	public:
		static MeshData* load(const String& file);
	private:
		MeshUtils() {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int MeshData::getNumVertices() 
	{ return vertices.size(); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int MeshData::getNumTriangles() 
	{ return triangles.size(); }
		
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector3f& MeshData::getVertexPosition(int i) 
	{ return vertices[i]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color& MeshData::getVertexColor(int i) 
	{ return colors[i]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector3f& MeshData::getVertexNormal(int i) 
	{ return normals[i]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Triangle& MeshData::getTriangle(int i) 
	{ return triangles[i]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector2f& MeshData::getTexCoords0(int i) 
	{ return texCoords0[i]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector2f& MeshData::getTexCoords1(int i) 
	{ return texCoords1[i]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector2f& MeshData::getTexCoords2(int i) 
	{ return texCoords2[i]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector2f& MeshData::getTexCoords3(int i) 
	{ return texCoords3[i]; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void MeshData::addTriangle(const Triangle& tri) 
	{ triangles.push_back(tri); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void MeshData::addVertex(const Vector3f& pos) 
	{
		myHasColors = false;
		myHasNormals = false;
		myIsDirty = true;
		vertices.push_back(pos);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void MeshData::addVertex(const Vector3f& pos, const Vector3f& normal) 
	{
		myHasColors = false;
		myHasNormals = true;
		myIsDirty = true;
		vertices.push_back(pos);
		normals.push_back(normal);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void MeshData::addVertex(const Vector3f& pos, const Vector3f& normal, const Color& color)
	{
		myHasColors = true;
		myHasNormals = true;
		myIsDirty = true;
		vertices.push_back(pos);
		colors.push_back(color);
		normals.push_back(normal);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void MeshData::addVertex(const Vector3f& pos, const Color& color)
	{
		myHasColors = true;
		myHasNormals = false;
		myIsDirty = true;
		vertices.push_back(pos);
		colors.push_back(color);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool MeshData::hasColors() 
	{ return myHasColors; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool MeshData::hasNormals() 
	{ return myHasNormals; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool MeshData::hasTexCoords0() 
	{ return myHasTexCoords0; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool MeshData::hasTexCoords1() 
	{ return myHasTexCoords1; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool MeshData::hasTexCoords2() 
	{ return myHasTexCoords2; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool MeshData::hasTexCoords3() 
	{ return myHasTexCoords3; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const AlignedBox3& MeshData::getBoundingBox() 
	{ 
		if(myIsDirty) update(); 
		return myBBox; 
	}

};


#endif 
