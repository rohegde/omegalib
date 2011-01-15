//Generated file - Edit roiFragmentShader.glsl!
#include <string>
static const std::string roiFragmentShader_glsl = "/* Copyright (c) 2009       Maxim Makhinya\n  *\n  * This library is free software; you can redistribute it and/or modify it under\n  * the terms of the GNU Lesser General Public License version 2.1 as published\n  * by the Free Software Foundation.\n  *  \n  * This library is distributed in the hope that it will be useful, but WITHOUT\n  * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS\n  * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more\n  * details.\n  * \n  * You should have received a copy of the GNU Lesser General Public License\n  * along with this library; if not, write to the Free Software Foundation, Inc.,\n  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.\n  */\n \n uniform sampler2DRect texture;\n \n void main(void)\n {\n     vec2  pos = (gl_FragCoord.xy - vec2( 0.5, 0.5 ))*16.0;\n \n     float bg  = 1.0;\n \n     float s;\n     for( float y = .0; y < 15.1; y+=2.0 )\n     {\n         if( bg < .5 )\n             break;\n         for( float x = .0; x < 15.1; x+=2.0 )\n         {\n             s = texture2DRect( texture, pos + vec2( x, y )).x;\n             if( s != 1.0 )\n             {\n                 bg = .0;\n                 break;\n             }\n         }\n     }\n     for( float y = 1.0; y < 15.1; y+=2.0 )\n     {\n         if( bg < .5 )\n             break;\n         for( float x = 1.0; x < 15.1; x+=2.0 )\n         {\n             s = texture2DRect( texture, pos + vec2( x, y )).x;\n \n             if( s != 1.0 )\n             {\n                 bg = .0;\n                 break;\n             }\n         }\n     }\n     for( float y = .0; y < 15.1; y+=2.0 )\n     {\n         if( bg < .5 )\n             break;\n         for( float x = 1.0; x < 15.1; x+=2.0 )\n         {\n             s = texture2DRect( texture, pos + vec2( x, y )).x;\n \n             if( s != 1.0 )\n             {\n                 bg = .0;\n                 break;\n             }\n         }\n     }\n     for( float y = 1.0; y < 15.1; y+=2.0 )\n     {\n         if( bg < .5 )\n             break;\n         for( float x = .0; x < 15.1; x+=2.0 )\n         {\n             s = texture2DRect( texture, pos + vec2( x, y )).x;\n \n             if( s != 1.0 )\n             {\n                 bg = .0;\n                 break;\n             }\n         }\n     }\n \n     gl_FragColor = vec4( bg, .0, .0, .0 );\n }\n \n ";
