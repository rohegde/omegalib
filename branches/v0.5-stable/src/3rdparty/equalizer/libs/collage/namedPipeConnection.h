
/* Copyright (c) 2005-2010, Stefan Eilemann <eile@equalizergraphics.com> 
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
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

#ifndef CO_NAMEDPIPECONNECTION_H
#define CO_NAMEDPIPECONNECTION_H

#include <co/base/os.h>
#include <co/base/buffer.h> // member
#include <co/base/thread.h> // for EQ_TS_VAR

#ifdef WIN32
#  include <co/connection.h>
#else
#  include "fdConnection.h"
#  include <netinet/in.h>
#endif


namespace co
{
    /**
     * A .
     */
    class NamedPipeConnection
#ifdef WIN32
        : public Connection
#else
        : public FDConnection
#endif
    {
    public:
        NamedPipeConnection();

        virtual bool connect();
        virtual bool listen();
        virtual void acceptNB();
        virtual ConnectionPtr acceptSync();
        
        virtual void close();


#ifdef WIN32
        virtual Notifier getNotifier() const {
            return _read.hEvent; }
#endif

    protected:
        virtual ~NamedPipeConnection();

        void _initAIOAccept();
        void _exitAIOAccept();
        void _initAIORead();
        void _exitAIORead();

#ifdef WIN32
        virtual void readNB( void* buffer, const uint64_t bytes );
        virtual int64_t readSync( void* buffer, const uint64_t bytes,
                                  const bool ignored );
        virtual int64_t write( const void* buffer, const uint64_t bytes );
#else

#endif

    private:
        bool _createNamedPipe();

#ifdef WIN32
        HANDLE _fd;

        bool _connectToNewClient( HANDLE hPipe ) ;

        // overlapped data structures
        OVERLAPPED _read;
        DWORD      _readDone;
        OVERLAPPED _write;

        EQ_TS_VAR( _recvThread );

        std::string _getFilename() const;
#endif
    };
}

#endif //CO_NAMEDPIPECONNECTION_H 
