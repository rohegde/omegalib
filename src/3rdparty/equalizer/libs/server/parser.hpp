
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     EQTOKEN_HEADER = 258,
     EQTOKEN_ASCII = 259,
     EQTOKEN_GLOBAL = 260,
     EQTOKEN_CONNECTION_SATTR_HOSTNAME = 261,
     EQTOKEN_CONNECTION_IATTR_TYPE = 262,
     EQTOKEN_CONNECTION_IATTR_PORT = 263,
     EQTOKEN_CONNECTION_SATTR_FILENAME = 264,
     EQTOKEN_CONNECTION_IATTR_BANDWIDTH = 265,
     EQTOKEN_CONFIG_FATTR_EYE_BASE = 266,
     EQTOKEN_CONFIG_IATTR_ROBUSTNESS = 267,
     EQTOKEN_NODE_SATTR_LAUNCH_COMMAND = 268,
     EQTOKEN_NODE_CATTR_LAUNCH_COMMAND_QUOTE = 269,
     EQTOKEN_NODE_IATTR_THREAD_MODEL = 270,
     EQTOKEN_NODE_IATTR_LAUNCH_TIMEOUT = 271,
     EQTOKEN_NODE_IATTR_HINT_STATISTICS = 272,
     EQTOKEN_PIPE_IATTR_HINT_THREAD = 273,
     EQTOKEN_PIPE_IATTR_HINT_CUDA_GL_INTEROP = 274,
     EQTOKEN_WINDOW_IATTR_HINT_STEREO = 275,
     EQTOKEN_WINDOW_IATTR_HINT_DOUBLEBUFFER = 276,
     EQTOKEN_WINDOW_IATTR_HINT_FULLSCREEN = 277,
     EQTOKEN_WINDOW_IATTR_HINT_DECORATION = 278,
     EQTOKEN_WINDOW_IATTR_HINT_SWAPSYNC = 279,
     EQTOKEN_WINDOW_IATTR_HINT_DRAWABLE = 280,
     EQTOKEN_WINDOW_IATTR_HINT_STATISTICS = 281,
     EQTOKEN_WINDOW_IATTR_HINT_SCREENSAVER = 282,
     EQTOKEN_WINDOW_IATTR_PLANES_COLOR = 283,
     EQTOKEN_WINDOW_IATTR_PLANES_ALPHA = 284,
     EQTOKEN_WINDOW_IATTR_PLANES_DEPTH = 285,
     EQTOKEN_WINDOW_IATTR_PLANES_STENCIL = 286,
     EQTOKEN_WINDOW_IATTR_PLANES_ACCUM = 287,
     EQTOKEN_WINDOW_IATTR_PLANES_ACCUM_ALPHA = 288,
     EQTOKEN_WINDOW_IATTR_PLANES_SAMPLES = 289,
     EQTOKEN_COMPOUND_IATTR_STEREO_MODE = 290,
     EQTOKEN_COMPOUND_IATTR_STEREO_ANAGLYPH_LEFT_MASK = 291,
     EQTOKEN_COMPOUND_IATTR_STEREO_ANAGLYPH_RIGHT_MASK = 292,
     EQTOKEN_COMPOUND_IATTR_UPDATE_FOV = 293,
     EQTOKEN_COMPOUND_IATTR_HINT_OFFSET = 294,
     EQTOKEN_CHANNEL_IATTR_HINT_STATISTICS = 295,
     EQTOKEN_CHANNEL_IATTR_HINT_SENDTOKEN = 296,
     EQTOKEN_SERVER = 297,
     EQTOKEN_CONFIG = 298,
     EQTOKEN_APPNODE = 299,
     EQTOKEN_NODE = 300,
     EQTOKEN_PIPE = 301,
     EQTOKEN_WINDOW = 302,
     EQTOKEN_ATTRIBUTES = 303,
     EQTOKEN_HINT_STEREO = 304,
     EQTOKEN_HINT_DOUBLEBUFFER = 305,
     EQTOKEN_HINT_FULLSCREEN = 306,
     EQTOKEN_HINT_DECORATION = 307,
     EQTOKEN_HINT_STATISTICS = 308,
     EQTOKEN_HINT_SENDTOKEN = 309,
     EQTOKEN_HINT_SWAPSYNC = 310,
     EQTOKEN_HINT_DRAWABLE = 311,
     EQTOKEN_HINT_THREAD = 312,
     EQTOKEN_HINT_CUDA_GL_INTEROP = 313,
     EQTOKEN_HINT_SCREENSAVER = 314,
     EQTOKEN_PLANES_COLOR = 315,
     EQTOKEN_PLANES_ALPHA = 316,
     EQTOKEN_PLANES_DEPTH = 317,
     EQTOKEN_PLANES_STENCIL = 318,
     EQTOKEN_PLANES_ACCUM = 319,
     EQTOKEN_PLANES_ACCUM_ALPHA = 320,
     EQTOKEN_PLANES_SAMPLES = 321,
     EQTOKEN_ON = 322,
     EQTOKEN_OFF = 323,
     EQTOKEN_AUTO = 324,
     EQTOKEN_FASTEST = 325,
     EQTOKEN_NICEST = 326,
     EQTOKEN_QUAD = 327,
     EQTOKEN_ANAGLYPH = 328,
     EQTOKEN_PASSIVE = 329,
     EQTOKEN_RED = 330,
     EQTOKEN_GREEN = 331,
     EQTOKEN_BLUE = 332,
     EQTOKEN_HORIZONTAL = 333,
     EQTOKEN_VERTICAL = 334,
     EQTOKEN_DFR = 335,
     EQTOKEN_DDS = 336,
     EQTOKEN_FRAMERATE = 337,
     EQTOKEN_DPLEX = 338,
     EQTOKEN_CHANNEL = 339,
     EQTOKEN_OBSERVER = 340,
     EQTOKEN_LAYOUT = 341,
     EQTOKEN_VIEW = 342,
     EQTOKEN_CANVAS = 343,
     EQTOKEN_SEGMENT = 344,
     EQTOKEN_COMPOUND = 345,
     EQTOKEN_LOADBALANCER = 346,
     EQTOKEN_DFREQUALIZER = 347,
     EQTOKEN_FRAMERATEEQUALIZER = 348,
     EQTOKEN_LOADEQUALIZER = 349,
     EQTOKEN_TREEEQUALIZER = 350,
     EQTOKEN_MONITOREQUALIZER = 351,
     EQTOKEN_VIEWEQUALIZER = 352,
     EQTOKEN_DAMPING = 353,
     EQTOKEN_CONNECTION = 354,
     EQTOKEN_NAME = 355,
     EQTOKEN_TYPE = 356,
     EQTOKEN_TCPIP = 357,
     EQTOKEN_SDP = 358,
     EQTOKEN_IB = 359,
     EQTOKEN_MCIP = 360,
     EQTOKEN_PGM = 361,
     EQTOKEN_RSP = 362,
     EQTOKEN_TEXTURE = 363,
     EQTOKEN_MEMORY = 364,
     EQTOKEN_FIXED = 365,
     EQTOKEN_HMD = 366,
     EQTOKEN_HOSTNAME = 367,
     EQTOKEN_INTERFACE = 368,
     EQTOKEN_LAUNCH_COMMAND = 369,
     EQTOKEN_LAUNCH_COMMAND_QUOTE = 370,
     EQTOKEN_LAUNCH_TIMEOUT = 371,
     EQTOKEN_PORT = 372,
     EQTOKEN_FILENAME = 373,
     EQTOKEN_TASK = 374,
     EQTOKEN_EYE = 375,
     EQTOKEN_EYE_BASE = 376,
     EQTOKEN_ROBUSTNESS = 377,
     EQTOKEN_THREAD_MODEL = 378,
     EQTOKEN_ASYNC = 379,
     EQTOKEN_DRAW_SYNC = 380,
     EQTOKEN_LOCAL_SYNC = 381,
     EQTOKEN_BUFFER = 382,
     EQTOKEN_CLEAR = 383,
     EQTOKEN_DRAW = 384,
     EQTOKEN_ASSEMBLE = 385,
     EQTOKEN_READBACK = 386,
     EQTOKEN_COLOR = 387,
     EQTOKEN_DEPTH = 388,
     EQTOKEN_CYCLOP = 389,
     EQTOKEN_LEFT = 390,
     EQTOKEN_RIGHT = 391,
     EQTOKEN_VIEWPORT = 392,
     EQTOKEN_DRAWABLE = 393,
     EQTOKEN_FBO_COLOR = 394,
     EQTOKEN_FBO_DEPTH = 395,
     EQTOKEN_FBO_STENCIL = 396,
     EQTOKEN_RANGE = 397,
     EQTOKEN_PERIOD = 398,
     EQTOKEN_PHASE = 399,
     EQTOKEN_PIXEL = 400,
     EQTOKEN_SUBPIXEL = 401,
     EQTOKEN_BANDWIDTH = 402,
     EQTOKEN_DEVICE = 403,
     EQTOKEN_WALL = 404,
     EQTOKEN_BOTTOM_LEFT = 405,
     EQTOKEN_BOTTOM_RIGHT = 406,
     EQTOKEN_TOP_LEFT = 407,
     EQTOKEN_PROJECTION = 408,
     EQTOKEN_ORIGIN = 409,
     EQTOKEN_DISTANCE = 410,
     EQTOKEN_FOV = 411,
     EQTOKEN_HPR = 412,
     EQTOKEN_LATENCY = 413,
     EQTOKEN_SWAPBARRIER = 414,
     EQTOKEN_NVGROUP = 415,
     EQTOKEN_NVBARRIER = 416,
     EQTOKEN_OUTPUTFRAME = 417,
     EQTOKEN_INPUTFRAME = 418,
     EQTOKEN_STEREO_MODE = 419,
     EQTOKEN_STEREO_ANAGLYPH_LEFT_MASK = 420,
     EQTOKEN_STEREO_ANAGLYPH_RIGHT_MASK = 421,
     EQTOKEN_UPDATE_FOV = 422,
     EQTOKEN_HINT_OFFSET = 423,
     EQTOKEN_PBUFFER = 424,
     EQTOKEN_FBO = 425,
     EQTOKEN_RGBA16F = 426,
     EQTOKEN_RGBA32F = 427,
     EQTOKEN_MODE = 428,
     EQTOKEN_2D = 429,
     EQTOKEN_ASSEMBLE_ONLY_LIMIT = 430,
     EQTOKEN_DB = 431,
     EQTOKEN_BOUNDARY = 432,
     EQTOKEN_ZOOM = 433,
     EQTOKEN_MONO = 434,
     EQTOKEN_STEREO = 435,
     EQTOKEN_STRING = 436,
     EQTOKEN_CHARACTER = 437,
     EQTOKEN_FLOAT = 438,
     EQTOKEN_INTEGER = 439,
     EQTOKEN_UNSIGNED = 440
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


    const char*             _string;
    char                    _character;
    int                     _int;
    unsigned                _unsigned;
    float                   _float;
    co::ConnectionType   _connectionType;
    eq::server::LoadEqualizer::Mode _loadEqualizerMode;
    eq::server::TreeEqualizer::Mode _treeEqualizerMode;
    float                   _viewport[4];



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE eqLoader_lval;


