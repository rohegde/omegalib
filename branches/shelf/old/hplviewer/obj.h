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
#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>
#include <map>
#include <istream>

#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>

#include <osgDB/ReaderWriter>


namespace obj
{

class Material
{
public:
    Material():
        ambient(0.2f,0.2f,0.2f,1.0f),
        diffuse(0.8f,0.8f,0.8f,1.0f),
        specular(0.0f,0.0f,0.0f,1.0f),
        emissive(0.0f,0.0f,0.0f,1.0f),
        sharpness(0.0f),
        illum(2),
        Tf(0.0f,0.0f,0.0f,1.0f),
        Ni(0),
        Ns(0),
        // textureReflection(false),
        alpha(1.0f) {}
    
    std::string name;
    
    osg::Vec4   ambient;
    osg::Vec4   diffuse;
    osg::Vec4   specular;
    osg::Vec4   emissive;
    float       sharpness;
    int         illum;            // read but not implemented (would need specific shaders or state manipulation)
    
    osg::Vec4   Tf;
    int         Ni;
    int         Ns; // shininess 0..1000

    // bool        textureReflection;
    float       alpha;

    class Map
    {
        // -o and -s (offset and scale) options supported for the maps
        // -clamp <on|off> is supported
        // -blendu, -blendv, -imfchan, not supported
        // -mm <base> <gain> is parsed but not actually used
        // -bm <bump_multiplier> is parsed but not used
    public:
        enum TextureMapType {
            DIFFUSE=0,
            OPACITY,
            AMBIENT,
            SPECULAR,
            SPECULAR_EXPONENT,
            BUMP,
            DISPLACEMENT,
            REFLECTION,        // read of a reflection map will also apply spherical texgen coordinates
            UNKNOWN            // UNKNOWN has to be the last
        };
        Map():
            type(UNKNOWN),
            name(""),
            uScale(1.0f),
            vScale(1.0f),
            uOffset(0.0f),
            vOffset(0.0f),
            clamp(false) {}


        TextureMapType type;
        std::string name;

        // Texture scale and offset, used for creating the texture matrix.
        // Reader only picks u and v from -s u v w, although all u v and w all need to be specified!
        // e.g. "map_Kd -s u v w <name>" is OK but "map_Kd -s u v <name>" is not, even though tex is only 2D
        float       uScale;
        float       vScale;
        float       uOffset;
        float       vOffset;

        // According to the spec, if clamping is off (default), the effect is a texture repeat
        // if clamping is on, then the effect is a decal texture; i.e. the border is transparent
        bool        clamp;
    };

    std::vector<Map> maps;

protected:
};

class Element : public osg::Referenced
{
public:

    typedef std::vector<int> IndexList;

    enum DataType
    {
        POINTS,
        POLYLINE,
        POLYGON
    };
    
    Element(DataType type):
        dataType(type) {}
        
    enum CoordinateCombination
    {
        VERTICES,
        VERTICES_NORMALS,
        VERTICES_TEXCOORDS,
        VERTICES_NORMALS_TEXCOORDS
    };
    
    CoordinateCombination getCoordinateCombination() const
    {
        if (vertexIndices.size()==normalIndices.size())
            return (vertexIndices.size()==texCoordIndices.size()) ? VERTICES_NORMALS_TEXCOORDS : VERTICES_NORMALS;
        else 
            return (vertexIndices.size()==texCoordIndices.size()) ?  VERTICES_TEXCOORDS : VERTICES;
    }
    
    DataType  dataType;  
    IndexList vertexIndices;
    IndexList normalIndices;
    IndexList texCoordIndices;
};

class ElementState
{
public:

    ElementState():
        coordinateCombination(Element::VERTICES),
        smoothingGroup(0) {}
        
    bool operator < (const ElementState& rhs) const
    {
        if (materialName<rhs.materialName) return true;
        else if (rhs.materialName<materialName) return false;

        if (objectName<rhs.objectName) return true;
        else if (rhs.objectName<objectName) return false;

        if (groupName<rhs.groupName) return true;
        else if (rhs.groupName<groupName) return false;

        if (coordinateCombination<rhs.coordinateCombination) return true;
        else if (rhs.coordinateCombination<coordinateCombination) return false;

        return (smoothingGroup<rhs.smoothingGroup);
    }


    std::string                     objectName;
    std::string                     groupName;
    std::string                     materialName;
    Element::CoordinateCombination  coordinateCombination;
    int                             smoothingGroup;
};

class Model
{
public:
    Model():
        currentElementList(0) {}
    
    void setDatabasePath(const std::string& path) { databasePath = path; }
    const std::string& getDatabasePath() const { return databasePath; }

    std::string lastComponent(const char* linep);
    bool readMTL(std::istream& fin);
    bool readOBJ(std::istream& fin, const osgDB::ReaderWriter::Options* options);
    
    bool readline(std::istream& fin, char* line, const int LINE_SIZE);
    void addElement(Element* element);
    
    osg::Vec3 averageNormal(const Element& element) const;
    osg::Vec3 computeNormal(const Element& element) const;
    bool needReverse(const Element& element) const;
    
    int remapVertexIndex(int vi) { return (vi<0) ? vertices.size()+vi : vi-1; }
    int remapNormalIndex(int vi) { return (vi<0) ? normals.size()+vi : vi-1; }
    int remapTexCoordIndex(int vi) { return (vi<0) ? texcoords.size()+vi : vi-1; }
    
    typedef std::map<std::string,Material>          MaterialMap;
    typedef std::vector< osg::Vec2 >                Vec2Array;
    typedef std::vector< osg::Vec3 >                Vec3Array;
    typedef std::vector< osg::ref_ptr<Element> >    ElementList;
    typedef std::map< ElementState,ElementList >    ElementStateMap;
    

    std::string     databasePath;
    MaterialMap     materialMap;
       
    Vec3Array       vertices;
    Vec3Array       normals;
    Vec2Array       texcoords;
    
    ElementState    currentElementState;
    
    ElementStateMap elementStateMap;
    ElementList*    currentElementList;

};

}

#endif
