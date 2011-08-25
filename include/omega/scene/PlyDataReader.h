/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *  [PLACE YOUR NAME AND MAIL HERE IF YOU CONTRIBUTED TO WRITE THIS SOURCE FILE]
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
 *---------------------------------------------------------------------------------------------------------------------
 * This file is a modified version of vertexData, eqPly, Equalizer.
 * Copyright (c) 2007, Tobias Wolf <twolf@access.unizh.ch>
 *               2009, Stefan Eilemann <eile@equalizergraphics.com>
 *********************************************************************************************************************/
#ifndef PLY_DATA_READER
#define PLY_DATA_READER

#include "omega/scene/MeshData.h"
#include "omega/scene/ply.h"

namespace omega 
{
namespace scene 
{
    /*  Holds the flat data and offers routines to read, scale and sort it.  */
    class OUTILS_API PlyDataReader: public MeshData
    {
    public:
        PlyDataReader();

        bool readPlyFile( const std::string& file );
        void sort( const uint start, const uint length, const Axis axis );
        void scale( const float baseSize = 2.0f );
        void calculateNormals( const bool vertexNormals = true );
        void calculateBoundingBox();
        const AlignedBox3& getBoundingBox() const { return _boundingBox; }
        Axis getLongestAxis( const size_t start, const size_t elements ) const;

        void useInvertedFaces() { _invertFaces = true; }

    private:
        void readVertices( PlyFile* file, const int nVertices, 
                           const bool readColors );
        void readTriangles( PlyFile* file, const int nFaces );

        AlignedBox3 _boundingBox;
        bool        _invertFaces;
    };
}; // namespace scene
} // namespace omega


#endif // MESH_VERTEXDATA_H
