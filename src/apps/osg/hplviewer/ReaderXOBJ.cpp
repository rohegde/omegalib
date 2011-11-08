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
 *-------------------------------------------------------------------------------------------------
 * Original code from:
 * Wavefront OBJ loader for Open Scene Graph
 *
 * Copyright (C) 2001,2007 Ulrich Hertlein <u.hertlein@sandbox.de>
 *
 * Modified by Robert Osfield to support per Drawable coord, normal and
 * texture coord arrays, bug fixes, and support for texture mapping.
 *
 * Writing support added 2007 by Stephan Huber, http://digitalmind.de, 
 * some ideas taken from the dae-plugin
 *
 * The Open Scene Graph (OSG) is a cross platform C++/OpenGL library for 
 * real-time rendering of large 3D photo-realistic models. 
 * The OSG homepage is http://www.openscenegraph.org/
 *************************************************************************************************/
#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <oengine.h>

#include <osg/Notify>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Vec3f>

#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexMat>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgUtil/TriStripVisitor>
#include <osgUtil/SmoothingVisitor>
#include <osgUtil/Tessellator>
#include <osgUtil/TangentSpaceGenerator>

#include <osg/PolygonMode>


#include "SolidEffect.h"
#include "SceneManager.h"
#include "obj.h"

using namespace hpl;

///////////////////////////////////////////////////////////////////////////////////////////////////
class ReaderXOBJ : public osgDB::ReaderWriter
{
public:
    ReaderXOBJ()
    {
        supportsExtension("obj","Alias Wavefront OBJ format");
        supportsOption("noTesselateLargePolygons","Do not do the default tesselation of large polygons");
        supportsOption("noTriStripPolygons","Do not do the default tri stripping of polygons");
        supportsOption("generateFacetNormals","generate facet normals for verticies without normals");
    }

    virtual const char* className() const { return "Wavefront OBJ Reader"; }
    virtual ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const;
    virtual ReadResult readNode(std::istream& fin, const Options* options) const;
    
protected:
     struct ObjOptionsStruct 
	 {
        bool noTesselateLargePolygons;
        bool noTriStripPolygons;
        bool generateFacetNormals;
    };

    typedef Dictionary<String, osgFX::Effect*> EffectMap;

    void buildEffectMap(obj::Model& model, EffectMap& em, ObjOptionsStruct& localOptions, const Options* options) const;
    osg::Geometry* convertElementListToGeometry(obj::Model& model, obj::Model::ElementList& elementList, ObjOptionsStruct& localOptions) const;
    osg::Node* convertModelToSceneGraph(obj::Model& model, ObjOptionsStruct& localOptions, const Options* options) const;
    ObjOptionsStruct parseOptions(const Options* options) const;
};

// register with Registry to instantiate the above reader/writer.
REGISTER_OSGPLUGIN(obj, ReaderXOBJ)

///////////////////////////////////////////////////////////////////////////////////////////////////
osg::Geometry* ReaderXOBJ::convertElementListToGeometry(obj::Model& model, obj::Model::ElementList& elementList, ObjOptionsStruct& localOptions) const
{
    
    unsigned int numVertexIndices = 0;
    unsigned int numNormalIndices = 0;
    unsigned int numTexCoordIndices = 0;
    
    unsigned int numPointElements = 0;
    unsigned int numPolylineElements = 0;
    unsigned int numPolygonElements = 0;

    obj::Model::ElementList::iterator itr;
        
    if (localOptions.generateFacetNormals == true) {
        for(itr=elementList.begin();
                itr!=elementList.end();
                    ++itr)
        {
            obj::Element& element = *(*itr);
            if (element.dataType==obj::Element::POINTS || element.dataType==obj::Element::POLYLINE)
                continue;
                        
            if (element.normalIndices.size() == 0) {
                // fill in the normals
                int a = element.vertexIndices[0];
                int b = element.vertexIndices[1];
                int c = element.vertexIndices[2];

                osg::Vec3f ab(model.vertices[b]);
                osg::Vec3f ac(model.vertices[c]);

                ab -= model.vertices[a];
                ac -= model.vertices[a];

                osg::Vec3f Norm( ab ^ ac );
                Norm.normalize();
                int normal_idx = model.normals.size();
                model.normals.push_back(Norm);

                for (unsigned i=0 ; i < element.vertexIndices.size() ; i++)
                    element.normalIndices.push_back(normal_idx);
            }
        }
    }
        
        
        
    for(itr=elementList.begin();
        itr!=elementList.end();
        ++itr)
    {
        obj::Element& element = *(*itr);

        numVertexIndices += element.vertexIndices.size();
        numNormalIndices += element.normalIndices.size();
        numTexCoordIndices += element.texCoordIndices.size();

        numPointElements += (element.dataType==obj::Element::POINTS) ? 1 : 0;
        numPolylineElements += (element.dataType==obj::Element::POLYLINE) ? 1 : 0;
        numPolygonElements += (element.dataType==obj::Element::POLYGON) ? 1 : 0;

    }

    if (numVertexIndices==0) return 0;
    
    if (numNormalIndices!=0 && numNormalIndices!=numVertexIndices)
    {
        OSG_NOTICE<<"Incorrect number of normals, ignore them"<<std::endl;
        numNormalIndices = 0;
    }
    
    if (numTexCoordIndices!=0 && numTexCoordIndices!=numVertexIndices)
    {
        OSG_NOTICE<<"Incorrect number of normals, ignore them"<<std::endl;
        numTexCoordIndices = 0;
    }
    
    osg::Vec3Array* vertices = numVertexIndices ? new osg::Vec3Array : 0;
    osg::Vec3Array* normals = numNormalIndices ? new osg::Vec3Array : 0;
    osg::Vec2Array* texcoords = numTexCoordIndices ? new osg::Vec2Array : 0;
    
    if (vertices) vertices->reserve(numVertexIndices);
    if (normals) normals->reserve(numNormalIndices);
    if (texcoords) texcoords->reserve(numTexCoordIndices);
    
    osg::Geometry* geometry = new osg::Geometry;
    if (vertices) geometry->setVertexArray(vertices);
    if (normals)
    {
        geometry->setNormalArray(normals);
        geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    }
    if (texcoords)
    {
        geometry->setTexCoordArray(0,texcoords);
    }
    

    if (numPointElements>0)
    {
        unsigned int startPos = vertices->size();
        unsigned int numPoints = 0;
        for(itr=elementList.begin();
            itr!=elementList.end();
            ++itr)
        {
            obj::Element& element = *(*itr);
            if (element.dataType==obj::Element::POINTS)
            {
                for(obj::Element::IndexList::iterator index_itr = element.vertexIndices.begin();
                    index_itr != element.vertexIndices.end();
                    ++index_itr)
                {
                    vertices->push_back(model.vertices[*index_itr]);
                    ++numPoints;
                }
                if (numNormalIndices)
                {
                    for(obj::Element::IndexList::iterator index_itr = element.normalIndices.begin();
                        index_itr != element.normalIndices.end();
                        ++index_itr)
                    {
                        normals->push_back(model.normals[*index_itr]);
                    }
                }
                if (numTexCoordIndices)
                {
                    for(obj::Element::IndexList::iterator index_itr = element.texCoordIndices.begin();
                        index_itr != element.texCoordIndices.end();
                        ++index_itr)
                    {
                        texcoords->push_back(model.texcoords[*index_itr]);
                    }
                }
            }
        }

        osg::DrawArrays* drawArrays = new osg::DrawArrays(GL_POINTS,startPos,numPoints);
        geometry->addPrimitiveSet(drawArrays);
    }
    
    if (numPolylineElements>0)
    {
        unsigned int startPos = vertices->size();
        osg::DrawArrayLengths* drawArrayLengths = new osg::DrawArrayLengths(GL_LINES,startPos);

        for(itr=elementList.begin();
            itr!=elementList.end();
            ++itr)
        {
            obj::Element& element = *(*itr);
            if (element.dataType==obj::Element::POLYLINE)
            {
                drawArrayLengths->push_back(element.vertexIndices.size());

                for(obj::Element::IndexList::iterator index_itr = element.vertexIndices.begin();
                    index_itr != element.vertexIndices.end();
                    ++index_itr)
                {
                    vertices->push_back(model.vertices[*index_itr]);
                }
                if (numNormalIndices)
                {
                    for(obj::Element::IndexList::iterator index_itr = element.normalIndices.begin();
                        index_itr != element.normalIndices.end();
                        ++index_itr)
                    {
                        normals->push_back(model.normals[*index_itr]);
                    }
                }
                if (numTexCoordIndices)
                {
                    for(obj::Element::IndexList::iterator index_itr = element.texCoordIndices.begin();
                        index_itr != element.texCoordIndices.end();
                        ++index_itr)
                    {
                        texcoords->push_back(model.texcoords[*index_itr]);
                    }
                }
            }
        }

        geometry->addPrimitiveSet(drawArrayLengths);

    }

    if (numPolygonElements>0)
    {
        unsigned int startPos = vertices->size();
        
        for(itr=elementList.begin();
            itr!=elementList.end();
            ++itr)
        {
            obj::Element& element = *(*itr);
            if (element.dataType==obj::Element::POLYGON)
            {
                if (element.vertexIndices.size()>4)
                {
                    osg::DrawArrays* drawArrays = new osg::DrawArrays(GL_POLYGON,startPos,element.vertexIndices.size());
                    startPos += element.vertexIndices.size();
                    geometry->addPrimitiveSet(drawArrays);
                }
                else
                {
                    osg::DrawArrays* drawArrays = new osg::DrawArrays(GL_TRIANGLE_FAN,startPos,element.vertexIndices.size());
                    startPos += element.vertexIndices.size();
                    geometry->addPrimitiveSet(drawArrays);
                }
                if (model.needReverse(element))
                {
                    // need to reverse so add to OSG arrays in same order as in OBJ, as OSG assume anticlockwise ordering.
                    for(obj::Element::IndexList::reverse_iterator index_itr = element.vertexIndices.rbegin();
                        index_itr != element.vertexIndices.rend();
                        ++index_itr)
                    {
                        vertices->push_back(model.vertices[*index_itr]);
                    }
                    if (numNormalIndices)
                    {
                        for(obj::Element::IndexList::reverse_iterator index_itr = element.normalIndices.rbegin();
                            index_itr != element.normalIndices.rend();
                            ++index_itr)
                        {
                            normals->push_back(model.normals[*index_itr]);
                        }
                    }


                    if (numTexCoordIndices)
                    {
                        for(obj::Element::IndexList::reverse_iterator index_itr = element.texCoordIndices.rbegin();
                            index_itr != element.texCoordIndices.rend();
                            ++index_itr)
                        {
                            texcoords->push_back(model.texcoords[*index_itr]);
                        }
                    }
                }
                else
                {
                    // no need to reverse so add to OSG arrays in same order as in OBJ.
                    for(obj::Element::IndexList::iterator index_itr = element.vertexIndices.begin();
                        index_itr != element.vertexIndices.end();
                        ++index_itr)
                    {
                        vertices->push_back(model.vertices[*index_itr]);
                    }
                    if (numNormalIndices)
                    {
                        for(obj::Element::IndexList::iterator index_itr = element.normalIndices.begin();
                            index_itr != element.normalIndices.end();
                            ++index_itr)
                        {
                            normals->push_back(model.normals[*index_itr]);
                        }
                    }
                    if (numTexCoordIndices)
                    {
                        for(obj::Element::IndexList::iterator index_itr = element.texCoordIndices.begin();
                            index_itr != element.texCoordIndices.end();
                            ++index_itr)
                        {
                            texcoords->push_back(model.texcoords[*index_itr]);
                        }
                    }
                }
            }
        }


    }
    
    return geometry;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ReaderXOBJ::buildEffectMap(obj::Model& model, EffectMap& em, ObjOptionsStruct& localOptions, const Options* options) const
{
	// Pass the current working dir to the data manager so it will be able to find texture files when creating effects.
	DataManager::getInstance()->setCurrentPath(model.getDatabasePath());

    for(obj::Model::MaterialMap::iterator itr = model.materialMap.begin();
        itr != model.materialMap.end();
        ++itr)
    {
        obj::Material& material = itr->second;
       
        //osg::ref_ptr< osg::StateSet > stateset = new osg::StateSet;

		// Get the material name using the name of the diffuse texture.
		String materialName = material.maps[obj::Material::Map::DIFFUSE].name;

		omega::DataManager::getInstance()->setCurrentPath(model.getDatabasePath());

		String materialPath;
		if(SceneManager::findMaterialFile(materialName, materialPath))
		{
			// Open the material XML file.
			TiXmlDocument doc(materialPath.c_str());
			if(doc.LoadFile())
			{
				String type = doc.RootElement()->FirstChildElement("Main")->Attribute("Type");
				StringUtils::toLowerCase(type);
				if(type == "soliddiffuse")
				{
					hpl::SolidEffect* fx = new hpl::SolidEffect();
					fx->load(doc.RootElement());
					em[material.name] = fx;
				}
				else
				{
					ofwarn("Unknown material type %1%", %type);
				}
			}
			else
			{
				ofwarn("Could not open material file %1%", %materialName);
			}
		}
		else
		{
			ofwarn("Could not find material file %1%", %materialName);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
osg::Node* ReaderXOBJ::convertModelToSceneGraph(obj::Model& model, ObjOptionsStruct& localOptions, const Options* options) const
{

    if (model.elementStateMap.empty()) return 0;

    osg::Group* group = new osg::Group;

    // set up the materials
    EffectMap fxs;
    buildEffectMap(model, fxs, localOptions, options);

    // go through the groups of related elements and build geometry from them.
    for(obj::Model::ElementStateMap::iterator itr=model.elementStateMap.begin();
        itr!=model.elementStateMap.end();
        ++itr)
    {
        const obj::ElementState& es = itr->first;
        obj::Model::ElementList& el = itr->second;

        osg::Geometry* geometry = convertElementListToGeometry(model,el,localOptions);

        if (geometry)
        {
            // tesseleate any large polygons
            if (!localOptions.noTesselateLargePolygons)
            {
                osgUtil::Tessellator tessellator;
                tessellator.retessellatePolygons(*geometry);
            }
            
            // tri strip polygons to improve graphics peformance
            if (!localOptions.noTriStripPolygons)
            {
                osgUtil::TriStripVisitor tsv;
                tsv.stripify(*geometry);
            }
            
            // if no normals present add them.
            if (localOptions.generateFacetNormals==false && (!geometry->getNormalArray() || geometry->getNormalArray()->getNumElements()==0))
            {
                osgUtil::SmoothingVisitor sv;
                sv.smooth(*geometry);
            }

            osg::Geode* geode = new osg::Geode;
			geode->setNodeMask(SceneManager::ReceivesShadowTraversalMask | SceneManager::CastsShadowTraversalMask);
            geode->addDrawable(geometry);

			// Set geometry texture coords
			geometry->setTexCoordArray(1, geometry->getTexCoordArray(0));
			geometry->setTexCoordArray(2, geometry->getTexCoordArray(0));
			geometry->setTexCoordArray(3, geometry->getTexCoordArray(0));
            
			// Generate tangent space for geometry
			osgUtil::TangentSpaceGenerator* tsg = new osgUtil::TangentSpaceGenerator();
			tsg->generate(geometry, 3);
			osg::Vec4Array* a_tangent = tsg->getTangentArray();
			geometry->setVertexAttribArray (6, a_tangent);
			geometry->setVertexAttribBinding (6, osg::Geometry::BIND_PER_VERTEX);

			// Set geometry material
			osgFX::Effect* fx = fxs[es.materialName];
			//osgFX::Effect* fx = NULL;
			if(fx != NULL)
			{
				fx->addChild(geode);
				group->addChild(fx);
			}
			else
			{
				//ofwarn("Could not find material %1%", %es.materialName);
				group->addChild(geode);
			}

			// Set object name.
            //if (es.objectName.empty())
            //{
            //    geode->setName(es.groupName);
            //}
            //else if (es.groupName.empty())
            //{
            //    geode->setName(es.objectName);
            //}
            //else
            //{
            //    geode->setName(es.groupName + std::string(":") + es.objectName);
            //}

        }
    }

    return group;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderXOBJ::ObjOptionsStruct ReaderXOBJ::parseOptions(const osgDB::ReaderWriter::Options* options) const
{
    ObjOptionsStruct localOptions;
    localOptions.noTesselateLargePolygons = false;
    localOptions.noTriStripPolygons = false;
    localOptions.generateFacetNormals = false;

    if (options!=NULL)
    {
        std::istringstream iss(options->getOptionString());
        std::string opt;
        while (iss >> opt)
        {
            // split opt into pre= and post=
            std::string pre_equals;
            std::string post_equals;

            size_t found = opt.find("=");
            if(found!=std::string::npos)
            {
                pre_equals = opt.substr(0,found);
                post_equals = opt.substr(found+1);
            } 
            else
            {
                pre_equals = opt;
            }

            if (pre_equals == "noTesselateLargePolygons")
            {
                localOptions.noTesselateLargePolygons = true;
            }                
            else if (pre_equals == "noTriStripPolygons")
            {
                localOptions.noTriStripPolygons = true;
            }
            else if (pre_equals == "generateFacetNormals")
            {
                localOptions.generateFacetNormals = true;
            }
        }
    }
    return localOptions;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
osgDB::ReaderWriter::ReadResult ReaderXOBJ::readNode(const std::string& file, const osgDB::ReaderWriter::Options* options) const
{
    std::string ext = osgDB::getLowerCaseFileExtension(file);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

    std::string fileName = osgDB::findDataFile( file, options );
    if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;
    
    
    osgDB::ifstream fin(fileName.c_str());
    if (fin)
    {
    
        // code for setting up the database path so that internally referenced file are searched for on relative paths. 
        osg::ref_ptr<Options> local_opt = options ? static_cast<Options*>(options->clone(osg::CopyOp::SHALLOW_COPY)) : new Options;
        local_opt->setDatabasePath(osgDB::getFilePath(fileName));

        obj::Model model;
        model.setDatabasePath(osgDB::getFilePath(fileName.c_str()));
        model.readOBJ(fin, local_opt.get());

        ObjOptionsStruct localOptions = parseOptions(options);

        osg::Node* node = convertModelToSceneGraph(model, localOptions, options);
        return node;
    }
    
    return ReadResult::FILE_NOT_HANDLED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
osgDB::ReaderWriter::ReadResult ReaderXOBJ::readNode(std::istream& fin, const Options* options) const
{
    if (fin)
    {
        obj::Model model;
        model.readOBJ(fin, options);
        
        ObjOptionsStruct localOptions = parseOptions(options);
        
        osg::Node* node = convertModelToSceneGraph(model, localOptions, options);
        return node;
    }
    
    return ReadResult::FILE_NOT_HANDLED;
}
