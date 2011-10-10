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
#include "oengine/ObjDataReader.h"
#include "omega/StringUtils.h"
#include "omega/DataManager.h"

#include "obj/obj_parser.hpp"

using namespace omega;
using namespace oengine;

using namespace std::tr1;
using namespace std::tr1::placeholders;

namespace oengine {
	class ObjParser
	{
	public:
		ObjParser(ObjDataReader* _reader): reader(_reader) {}

		void parse(DataInfo& data);

		void geometric_vertex_callback(obj::float_type x, obj::float_type y, obj::float_type z);
		void triangular_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3);
		void triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3);
		void triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3);
		void triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3);
		void quadrilateral_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3, obj::index_type v4);
		void quadrilateral_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3, const obj::index_2_tuple_type& v4_vt4);
		void quadrilateral_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3, const obj::index_2_tuple_type& v4_vn4);
		void quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3, const obj::index_3_tuple_type& v4_vt4_vn4);
		void polygonal_face_geometric_vertices_begin_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3);
		void polygonal_face_geometric_vertices_vertex_callback(obj::index_type v);
		void polygonal_face_geometric_vertices_end_callback();
		void polygonal_face_geometric_vertices_texture_vertices_begin_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3);
		void polygonal_face_geometric_vertices_texture_vertices_vertex_callback(const obj::index_2_tuple_type& v_vt);
		void polygonal_face_geometric_vertices_texture_vertices_end_callback();
		void polygonal_face_geometric_vertices_vertex_normals_begin_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3);
		void polygonal_face_geometric_vertices_vertex_normals_vertex_callback(const obj::index_2_tuple_type& v_vn);
		void polygonal_face_geometric_vertices_vertex_normals_end_callback();
		void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3);
		void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback(const obj::index_3_tuple_type& v_vt_vn);
		void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback();
	private:
		ObjDataReader* reader;
	};
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::parse(DataInfo& info)
{
	obj::obj_parser parser(obj::obj_parser::triangulate_faces);
	parser.geometric_vertex_callback(bind(&ObjParser::geometric_vertex_callback, this, _1, _2, _3));
	parser.face_callbacks(
	std::tr1::bind(&ObjParser::triangular_face_geometric_vertices_callback, this, _1, _2, _3),
	std::tr1::bind(&ObjParser::triangular_face_geometric_vertices_texture_vertices_callback, this, _1, _2, _3),
	std::tr1::bind(&ObjParser::triangular_face_geometric_vertices_vertex_normals_callback, this, _1, _2, _3),
	std::tr1::bind(&ObjParser::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback, this, _1, _2, _3),
	std::tr1::bind(&ObjParser::quadrilateral_face_geometric_vertices_callback, this, _1, _2, _3, _4),
	std::tr1::bind(&ObjParser::quadrilateral_face_geometric_vertices_texture_vertices_callback, this, _1, _2, _3, _4),
	std::tr1::bind(&ObjParser::quadrilateral_face_geometric_vertices_vertex_normals_callback, this, _1, _2, _3, _4),
	std::tr1::bind(&ObjParser::quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback, this, _1, _2, _3, _4),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_begin_callback, this, _1, _2, _3),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_vertex_callback, this, _1),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_end_callback, this),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_texture_vertices_begin_callback, this, _1, _2, _3),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_texture_vertices_vertex_callback, this, _1),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_texture_vertices_end_callback, this),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_vertex_normals_begin_callback, this, _1, _2, _3),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_vertex_normals_vertex_callback, this, _1),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_vertex_normals_end_callback, this),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback, this, _1, _2, _3),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback, this, _1),
	std::tr1::bind(&ObjParser::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback, this)
	);
	parser.parse(info.path);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::geometric_vertex_callback(obj::float_type x, obj::float_type y, obj::float_type z)
{
    reader->vertices.push_back(Vector3f( x, y, z ));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::triangular_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3)
{
    reader->triangles.push_back( Triangle( v1, v2, v3 ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3)
{
	omsg("triangular_face_geometric_vertices_texture_vertices_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3)
{
	omsg("triangular_face_geometric_vertices_vertex_normals_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3)
{
	omsg("triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::quadrilateral_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3, obj::index_type v4)
{
	omsg("quadrilateral_face_geometric_vertices_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::quadrilateral_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3, const obj::index_2_tuple_type& v4_vt4)
{
	omsg("quadrilateral_face_geometric_vertices_texture_vertices_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::quadrilateral_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3, const obj::index_2_tuple_type& v4_vn4)
{
	omsg("quadrilateral_face_geometric_vertices_vertex_normals_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3, const obj::index_3_tuple_type& v4_vt4_vn4)
{
	omsg("quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_begin_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3)
{
	omsg("polygonal_face_geometric_vertices_begin_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_vertex_callback(obj::index_type v)
{
	omsg("polygonal_face_geometric_vertices_vertex_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_end_callback()
{
	omsg("polygonal_face_geometric_vertices_end_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_texture_vertices_begin_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3)
{
	omsg("polygonal_face_geometric_vertices_texture_vertices_begin_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_texture_vertices_vertex_callback(const obj::index_2_tuple_type& v_vt)
{
	omsg("polygonal_face_geometric_vertices_texture_vertices_vertex_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_texture_vertices_end_callback()
{
	omsg("polygonal_face_geometric_vertices_texture_vertices_end_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_vertex_normals_begin_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3)
{
	omsg("polygonal_face_geometric_vertices_vertex_normals_begin_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_vertex_normals_vertex_callback(const obj::index_2_tuple_type& v_vn)
{
	omsg("polygonal_face_geometric_vertices_vertex_normals_vertex_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_vertex_normals_end_callback()
{
	omsg("polygonal_face_geometric_vertices_vertex_normals_end_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3)
{
	omsg("polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback(const obj::index_3_tuple_type& v_vt_vn)
{
	omsg("polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ObjParser::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback()
{
	omsg("polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ObjDataReader::ObjDataReader()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool ObjDataReader::readFile( const std::string& filename )
{
	DataManager* dm = SystemManager::instance()->getDataManager();
	DataInfo di = dm->getInfo(filename);
	if(!di.isNull())
	{
		ObjParser parser(this);
		parser.parse(di);
		return true;
	}
    return false;
}

