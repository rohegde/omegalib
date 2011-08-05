
/* Copyright (c) 2009-2010, Cedric Stalder <cedric.stalder@gmail.com> 
 *               2009-2010, Stefan Eilemann <eile@equalizergraphics.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
 
#ifndef EQ_PLUGIN_COMPRESSOR
#define EQ_PLUGIN_COMPRESSOR 

#include <co/plugins/compressor.h>
#include <co/api.h>
#include <co/base/os.h>
#include <co/base/buffer.h>
#include "../../collage/compressor/compressor.h"
#include <vector>
#ifdef _WIN32
#  include <malloc.h>
#endif



/**
 * @file client/compressor/compressor.h
 * 
 * Compression plugin provided with Equalizer.
 */
namespace eq
{
namespace plugin
{
    class Compressor
    {
    public:
        typedef void  (*CompressorGetInfo_t)( EqCompressorInfo* const );
        typedef void* (*NewCompressor_t)( const unsigned );
        typedef void (*Decompress_t)( const void* const*, const 
                                      eq_uint64_t* const,
                                      const unsigned, void* const, 
                                      const eq_uint64_t, const bool );
        typedef bool ( *IsCompatible_t ) ( const GLEWContext* );

        typedef co::plugin::Compressor::Functions Functions;
#ifdef false
        struct Functions
        {
            Functions( const unsigned name, CompressorGetInfo_t getInfo,
                       NewCompressor_t newCompressor,
                       NewCompressor_t newDecompressor,
                       Decompress_t decompress, IsCompatible_t isCompatible );

            unsigned             name;
            CompressorGetInfo_t  getInfo;
            NewCompressor_t      newCompressor;
            NewCompressor_t      newDecompressor;
            Decompress_t         decompress;
            IsCompatible_t       isCompatible;
        };
#endif


        /** Construct a new compressor. */
        Compressor();
        virtual ~Compressor();

        /**
         * Compress data.
         *
         * @param inData data to compress.
         * @param nPixels number data to compress.
         * @param useAlpha use alpha channel in compression.
         */
        virtual void compress( const void* const inData,
                               const eq_uint64_t nPixels, 
                               const bool useAlpha ) { EQDONTCALL; };

        typedef co::base::Bufferb Result;
        typedef std::vector< Result* > ResultVector;

        /** @return the vector containing the result data. */
        const ResultVector& getResults() const { return _results; }

        /** @return the number of result items produced. */
        unsigned getNResults() const { return _nResults; }

        /**
         * Transfer frame buffer data into main memory.
         * 
         * @param glewContext the initialized GLEW context describing
         *                    corresponding to the current OpenGL context.
         * @param inDims the dimensions of the input data (x, w, y, h).
         * @param source texture name to process.
         * @param flags capability flags for the compression (see description).
         * @param outDims the dimensions of the output data (see description).
         * @param out the pointer to the output data.
         */        
        virtual void download( const GLEWContext* glewContext,
                               const eq_uint64_t  inDims[4],
                               const unsigned     source,
                               const eq_uint64_t  flags,
                               eq_uint64_t        outDims[4],
                               void**             out ) { EQDONTCALL; }

        /**
         * Transfer data from main memory into GPU memory.
         *
         * @param glewContext the initialized GLEW context describing
         *                    corresponding to the current OpenGL context.
         * @param buffer the datas input.
         * @param inDims the dimension of data in the frame buffer.
         * @param flags capability flags for the compression.
         * @param outDims the result data size
         * @param destination the destination texture name.
         */
        virtual void upload( const GLEWContext* glewContext, 
                             const void*        buffer,
                             const eq_uint64_t  inDims[4],
                             const eq_uint64_t  flags,
                             const eq_uint64_t  outDims[4],  
                             const unsigned     destination ) { EQDONTCALL; }

        /** @internal Register a new plugin engine. */
        static void registerEngine( const Functions& functions );

    protected:
        ResultVector _results;  //!< The compressed data
        unsigned _nResults;     //!< Number of elements used in _results
    };
}
}

#endif // EQ_PLUGIN_COMPRESSOR

