//Generated file - Edit yuv420readback.glsl!
#include <string>
static const std::string yuv420readback_glsl = "/* Copyright (c) 2009       Maxim Makhinya\n  *\n  * This library is free software; you can redistribute it and/or modify it under\n  * the terms of the GNU Lesser General Public License as published by the Free\n  * Software Foundation; either version 2.1 of the License, or (at your option)\n  * any later version.\n  *  \n  * This library is distributed in the hope that it will be useful, but WITHOUT\n  * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS\n  * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more\n  * details.\n  * \n  * You should have received a copy of the GNU Lesser General Public License\n  * along with this library; if not, write to the Free Software Foundation, Inc.,\n  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.\n  */\n \n uniform sampler2DRect color;\n \n vec4 yMul = vec4( 0.299 , 0.587 , 0.114 , 0.0 );\n vec4 uMul = vec4(-0.147 ,-0.289 , 0.436 , 0.0 );\n vec4 vMul = vec4( 0.4996,-0.4184,-0.0812, 0.0 );\n /*vec4 vMul = vec4( 0.2896,-0.2084,-0.0802, 0.0 );*/\n \n vec4 test = vec4( 1.0, 1.0, 1.0, 0.0 );\n \n void main(void)\n {\n     vec2 pos = gl_FragCoord.xy - vec2( 0.5, 0.5 );\n     pos.x = pos.x * 2.0;\n \n     float odd = fract( pos.y*0.5 );\n     float hor;\n     if( odd < 0.5 )\n         hor =  1.0;\n     else\n         hor = -1.0;\n \n     vec4 p1 = texture2DRect( color, pos );\n     vec4 p2 = texture2DRect( color, pos + vec2( 1.0, 0.0 ));\n     vec4 p3 = texture2DRect( color, pos + vec2( 0.0, hor ));\n     vec4 p4 = texture2DRect( color, pos + vec2( 1.0, hor ));\n \n     float n = 0.0;\n     vec4  p = vec4( .0, .0, .0, .0 );\n \n     if( dot( p1, test ) > .0 ) { n += 1.0; p += p1; }\n     if( dot( p2, test ) > .0 ) { n += 1.0; p += p2; }\n     if( dot( p3, test ) > .0 ) { n += 1.0; p += p3; }\n     if( dot( p4, test ) > .0 ) { n += 1.0; p += p4; }\n \n     float cVal = 0.0;\n     if( n > 0.0 )\n     {\n         p /= n;\n \n         if( odd < 0.5 )\n             cVal = dot(p,uMul);\n         else\n             cVal = dot(p,vMul);\n     }\n //    cVal = 0.0;\n     gl_FragColor = vec4( dot(p1,yMul), dot(p2,yMul), cVal+0.5, (p1.a+p2.a)/2.0 );\n }\n \n ";
