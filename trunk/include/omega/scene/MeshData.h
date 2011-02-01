/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
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
 *********************************************************************************************************************/
#ifndef MESH_DATA
#define MESH_DATA

#include "omega/osystem.h"
#include "omega/Color.h"

namespace omega 
{
namespace scene 
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Used to store mesh data loaded from a file in a common format, before transfering it to vertex and index 
	//! buffers and material specifications. Mesh readers either derive from this class, or have a method that returns
	//! an instance of it.
    class MeshData
    {
    public:
		int getNumVertices() { return vertices.size(); }
		int getNumTriangles() { return triangles.size(); }
		
		Vector3f& getVertexPosition(int i) { return vertices[i]; }
		Color& getVertexColor(int i) { return colors[i]; }
		Vector3f& getVertexNormal(int i) { return normals[i]; }
		Triangle& getTriangle(int i) { return triangles[i]; }

		void addTriangle(const Triangle& tri) 
		{ 
			triangles.push_back(tri); 
		}

		void addVertex(const Vector3f& pos) 
		{
			myHasColors = false;
			myHasNormals = false;
			myIsDirty = true;
			vertices.push_back(pos);
		}

		void addVertex(const Vector3f& pos, const Vector3f& normal) 
		{
			myHasColors = false;
			myHasNormals = true;
			myIsDirty = true;
			vertices.push_back(pos);
			normals.push_back(normal);
		}

		void addVertex(const Vector3f& pos, const Vector3f& normal, const Color& color)
		{
			myHasColors = true;
			myHasNormals = true;
			myIsDirty = true;
			vertices.push_back(pos);
			colors.push_back(color);
			normals.push_back(normal);
		}

		void addVertex(const Vector3f& pos, const Color& color)
		{
			myHasColors = true;
			myHasNormals = false;
			myIsDirty = true;
			vertices.push_back(pos);
			colors.push_back(color);
		}

		//void addVertex(const Vector3f& pos, const Vector2f& tex) {/*TODO*/ }
		//void addVertex(const Vector3f& pos, const Vector3f& normal, const Vector2f& tex) {/*TODO*/ }
		//void addVertex(const Vector3f& pos, const Vector3f& normal, const Color& color, const Vector2f& tex) {/*TODO*/ }
		//void addVertex(const Vector3f& pos, const Color& color, const Vector2f& tex) {/*TODO*/ }

		bool hasColors() { return myHasColors; }
		bool hasNormals() { return myHasNormals; }

		const AxisAlignedBox& getBoundingBox() 
		{ 
			if(myIsDirty) update(); 
			return myBBox; 
		}

		void clear() 
		{
			vertices.clear();
			colors.clear();
			normals.clear();
			triangles.clear();
			myHasColors = false;
			myHasNormals = false;
			myIsDirty = true;
		}
	
	protected:
        std::vector< Vector3f >   vertices;
        std::vector< Color >    colors;
        std::vector< Vector3f >   normals;
        std::vector< Triangle > triangles;

		bool myHasColors;
		bool myHasNormals;

		bool myIsDirty;

	private:
		void update()
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

	private:
		AxisAlignedBox myBBox;
    };
}; // namespace scene
} // namespace omega


#endif 
