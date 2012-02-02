
/* Copyright (c) 2009, Stefan Eilemann <eile@equalizergraphics.com> 
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

#ifndef EQ_LAYOUT_H
#define EQ_LAYOUT_H

#include <eq/client/object.h>         // base class
#include <eq/client/types.h>
#include <eq/client/visitorResult.h>  // enum

#include <string>

namespace eq
{
namespace server
{
    class Layout;
}
    class Config;
    class LayoutVisitor;

    /**
     * A Layout groups one or more View which logically belong together. A
     * layout is applied to a Canvas. If no layout is applied to a canvas,
     * nothing is rendered on this canvas, i.e, the canvas is inactive. The
     * layout assignment can be changed at run-time by the application. The
     * intersection between views and segments defines which output
     * (sub-)channels are available. These channels are typically used as
     * destination channels in a compound. They are automatically created during
     * configuration load.
     */
    class Layout : public Object
    {
    public:
        /** 
         * Constructs a new Layout.
         */
        EQ_EXPORT Layout();

        /** Destruct this layout. */
        EQ_EXPORT virtual ~Layout();

        /**
         * @name Data Access
         */
        //@{
        /** Get the list of views. */
        const ViewVector& getViews() const { return _views; }

        Config* getConfig() { return _config; }
        const Config* getConfig() const { return _config; }
        //@}

        /**
         * @name Operations
         */
        //@{
        /** 
         * Traverse this layout and all children using a layout visitor.
         * 
         * @param visitor the visitor.
         * @return the result of the visitor traversal.
         */
        EQ_EXPORT VisitorResult accept( LayoutVisitor& visitor );

        /** Deregister this layout, and all children, from its net::Session.*/
        EQ_EXPORT virtual void deregister();
        //@}
        
    protected:
        /** @sa Object::deserialize */
        EQ_EXPORT virtual void deserialize( net::DataIStream& is, 
                                            const uint64_t dirtyBits );

        enum DirtyBits
        {
            DIRTY_VIEWS      = Object::DIRTY_CUSTOM << 0,
            DIRTY_FILL1      = Object::DIRTY_CUSTOM << 1,
            DIRTY_FILL2      = Object::DIRTY_CUSTOM << 2,
            DIRTY_CUSTOM     = Object::DIRTY_CUSTOM << 3
        };
        friend class server::Layout;

    private:
        /** The parent Config. */
        Config* _config;
        friend class Config;

        /** Child views on this layout. */
        ViewVector _views;

        union // placeholder for binary-compatible changes
        {
            char dummy[64];
        };

        void _addView( View* view );
        bool _removeView( View* view );
    };

    std::ostream& operator << ( std::ostream& os, const Layout* layout);
}
#endif // EQ_LAYOUT_H