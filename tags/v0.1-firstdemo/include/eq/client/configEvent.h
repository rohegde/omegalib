
/* Copyright (c) 2006-2009, Stefan Eilemann <eile@equalizergraphics.com> 
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

#ifndef EQ_CONFIGEVENT_H
#define EQ_CONFIGEVENT_H

#include <eq/client/event.h>   // member
#include <eq/client/packets.h> // base class

namespace eq
{
    /** 
     * A config event.
     *
     * Config events are generated by the event processing functions on the
     * render nodes and send to the config thread, i.e., to the application
     * using Config::sendEvent(). The config processes events at the end of the
     * frame, either non-blocking or blocking (event-driven). Config events are
     * typically application-specific. The events defined below are for the
     * default implementation, which sends the window system events more or less
     * unprocessed to the application.
     *
     * When subclassing, make sure to set the packet size.
     */
    struct ConfigEvent : public ConfigPacket
    {
    public:
        ConfigEvent()
            {
                command = CMD_CONFIG_EVENT;
                size    = sizeof( ConfigEvent );
            }
        
        Event data;
    };

    EQ_EXPORT std::ostream& operator << ( std::ostream& os, 
                                          const ConfigEvent* event );
}

#endif // EQ_CONFIGEVENT_H

