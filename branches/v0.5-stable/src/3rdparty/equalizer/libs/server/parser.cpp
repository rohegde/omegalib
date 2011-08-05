
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         eqLoader_parse
#define yylex           eqLoader_lex
#define yyerror         eqLoader_error
#define yylval          eqLoader_lval
#define yychar          eqLoader_char
#define yydebug         eqLoader_debug
#define yynerrs         eqLoader_nerrs


/* Copy the first part of user declarations.  */


#include "loader.h"

#include "canvas.h"
#include "channel.h"
#include "compound.h"
#include "equalizers/dfrEqualizer.h"
#include "equalizers/framerateEqualizer.h"
#include "equalizers/loadEqualizer.h"
#include "equalizers/treeEqualizer.h"
#include "equalizers/monitorEqualizer.h"
#include "equalizers/viewEqualizer.h"
#include "frame.h"
#include "global.h"
#include "layout.h"
#include "node.h"
#include "observer.h"
#include "pipe.h"
#include "segment.h"
#include "server.h"
#include "swapBarrier.h"
#include "view.h"
#include "window.h"

#include <eq/fabric/paths.h>
#include <co/base/os.h>
#include <co/base/file.h>
#include <string>
#pragma warning(disable: 4065)

    namespace eq
    {
    namespace loader
    {
        static eq::server::Loader*      loader = 0;
        std::string filename;

        static eq::server::ServerPtr    server;
        static eq::server::Config*      config = 0;
        static eq::server::Node*        node = 0;
        static eq::server::Pipe*        eqPipe = 0; // avoid name clash
        static eq::server::Window*      window = 0;
        static eq::server::Channel*     channel = 0;
        static eq::server::Layout*      layout = 0;
        static eq::server::View*        view = 0;
        static eq::server::Canvas*      canvas = 0;
        static eq::server::Segment*     segment = 0;
        static eq::server::Observer*    observer = 0;
        static eq::server::Compound*    eqCompound = 0; // avoid name clash
        static eq::server::DFREqualizer* dfrEqualizer = 0;
        static eq::server::LoadEqualizer* loadEqualizer = 0;
        static eq::server::TreeEqualizer* treeEqualizer = 0;
        static eq::server::SwapBarrier* swapBarrier = 0;
        static eq::server::Frame*       frame = 0;
        static co::ConnectionDescriptionPtr connectionDescription;
        static eq::fabric::Wall         wall;
        static eq::fabric::Projection   projection;
        static uint32_t                 flags = 0;
    }
    }

    using namespace co::base;
    using namespace eq::loader;

    int eqLoader_lex();

    #define yylineno eqLoader_lineno
    void yyerror( const char *errmsg );
    extern char* yytext;
    extern FILE*       yyin;
    extern const char* yyinString;
    extern int yylineno;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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


/* Copy the second part of user declarations.  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1031

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  192
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  149
/* YYNRULES -- Number of rules.  */
#define YYNRULES  407
/* YYNRULES -- Number of states.  */
#define YYNSTATES  690

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   440

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     190,   191,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   188,     2,   189,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   186,     2,   187,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,    11,    12,    16,    21,    22,    23,
      26,    29,    32,    35,    38,    41,    44,    47,    50,    53,
      56,    59,    62,    65,    68,    71,    74,    77,    80,    83,
      86,    89,    92,    95,    98,   101,   104,   107,   110,   113,
     116,   119,   122,   125,   128,   131,   134,   136,   138,   140,
     142,   144,   146,   148,   149,   156,   157,   160,   161,   167,
     169,   172,   173,   179,   180,   183,   185,   188,   190,   192,
     194,   196,   199,   204,   205,   208,   211,   214,   216,   218,
     219,   225,   226,   232,   233,   236,   239,   241,   243,   248,
     249,   255,   256,   259,   262,   265,   268,   271,   274,   277,
     278,   281,   284,   287,   290,   293,   296,   297,   303,   304,
     307,   309,   314,   317,   320,   323,   326,   327,   330,   333,
     336,   337,   343,   344,   347,   349,   354,   357,   360,   361,
     364,   367,   370,   373,   376,   379,   382,   385,   388,   391,
     394,   397,   400,   403,   406,   409,   410,   416,   417,   420,
     423,   428,   431,   432,   438,   439,   442,   445,   448,   449,
     455,   456,   459,   462,   465,   466,   472,   473,   476,   479,
     481,   482,   488,   489,   492,   495,   496,   500,   503,   505,
     507,   510,   513,   515,   517,   518,   524,   525,   528,   531,
     534,   537,   540,   542,   544,   546,   547,   553,   554,   557,
     560,   563,   564,   570,   573,   575,   577,   578,   581,   583,
     585,   587,   588,   594,   595,   598,   600,   603,   606,   609,
     610,   616,   617,   623,   624,   630,   633,   639,   642,   645,
     651,   659,   665,   667,   669,   671,   673,   675,   677,   679,
     684,   685,   690,   691,   694,   697,   700,   703,   706,   709,
     712,   715,   718,   719,   722,   724,   726,   728,   730,   731,
     734,   736,   738,   740,   741,   744,   746,   748,   749,   752,
     754,   756,   758,   759,   765,   766,   769,   776,   783,   790,
     793,   795,   797,   798,   804,   805,   808,   815,   818,   824,
     831,   836,   837,   840,   843,   846,   849,   852,   858,   861,
     863,   865,   867,   869,   871,   873,   875,   877,   879,   881,
     883,   885,   887,   888,   894,   898,   899,   905,   906,   912,
     916,   920,   921,   924,   927,   930,   931,   934,   937,   943,
     946,   949,   952,   954,   956,   958,   960,   961,   964,   967,
     973,   976,   979,   981,   983,   985,   987,   988,   994,   995,
     998,  1001,  1004,  1007,  1008,  1014,  1015,  1021,  1022,  1025,
    1028,  1031,  1034,  1035,  1041,  1047,  1049,  1051,  1052,  1055,
    1058,  1061,  1064,  1067,  1070,  1077,  1081,  1082,  1084,  1087,
    1089,  1091,  1093,  1095,  1097,  1099,  1101,  1103,  1105,  1107,
    1109,  1111,  1113,  1115,  1117,  1119,  1121,  1123,  1125,  1127,
    1129,  1131,  1133,  1135,  1137,  1139,  1141,  1143
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     193,     0,    -1,   194,   195,   198,    -1,   194,   195,   204,
      -1,    -1,     3,   338,     4,    -1,     5,   186,   196,   187,
      -1,    -1,    -1,   196,   195,    -1,     6,   336,    -1,     7,
     197,    -1,     8,   340,    -1,     9,   336,    -1,    10,   340,
      -1,    11,   338,    -1,    12,   335,    -1,    13,   336,    -1,
      14,   337,    -1,    15,   335,    -1,    16,   340,    -1,    17,
     335,    -1,    18,   335,    -1,    19,   335,    -1,    20,   335,
      -1,    21,   335,    -1,    22,   335,    -1,    23,   335,    -1,
      24,   335,    -1,    25,   335,    -1,    26,   335,    -1,    27,
     335,    -1,    28,   335,    -1,    29,   335,    -1,    30,   335,
      -1,    31,   335,    -1,    32,   335,    -1,    33,   335,    -1,
      34,   335,    -1,    40,   335,    -1,    41,   335,    -1,    35,
     335,    -1,    36,   332,    -1,    37,   332,    -1,    38,   335,
      -1,    39,   335,    -1,   102,    -1,   103,    -1,   104,    -1,
      46,    -1,   105,    -1,   106,    -1,   107,    -1,    -1,    42,
     186,   199,   200,   203,   187,    -1,    -1,   200,   201,    -1,
      -1,    99,   186,   202,   219,   187,    -1,   204,    -1,   203,
     204,    -1,    -1,    43,   186,   205,   206,   187,    -1,    -1,
     206,   207,    -1,   210,    -1,   100,   336,    -1,   242,    -1,
     246,    -1,   256,    -1,   267,    -1,   158,   340,    -1,    48,
     186,   208,   187,    -1,    -1,   208,   209,    -1,   121,   338,
      -1,   122,   335,    -1,   213,    -1,   211,    -1,    -1,    45,
     186,   212,   215,   187,    -1,    -1,    44,   186,   214,   215,
     187,    -1,    -1,   215,   216,    -1,   100,   336,    -1,   217,
      -1,   223,    -1,    48,   186,   221,   187,    -1,    -1,    99,
     186,   218,   219,   187,    -1,    -1,   219,   220,    -1,   101,
     197,    -1,   112,   336,    -1,   113,   336,    -1,   117,   340,
      -1,   147,   340,    -1,   118,   336,    -1,    -1,   221,   222,
      -1,   114,   336,    -1,   115,   337,    -1,   123,   335,    -1,
     116,   335,    -1,    53,   335,    -1,    -1,    46,   186,   224,
     225,   187,    -1,    -1,   225,   226,    -1,   229,    -1,    48,
     186,   227,   187,    -1,   100,   336,    -1,   117,   340,    -1,
     148,   340,    -1,   137,   331,    -1,    -1,   227,   228,    -1,
      57,   335,    -1,    58,   335,    -1,    -1,    47,   186,   230,
     231,   187,    -1,    -1,   231,   232,    -1,   235,    -1,    48,
     186,   233,   187,    -1,   100,   336,    -1,   137,   331,    -1,
      -1,   233,   234,    -1,    49,   335,    -1,    50,   335,    -1,
      51,   335,    -1,    52,   335,    -1,    55,   335,    -1,    56,
     335,    -1,    53,   335,    -1,    59,   335,    -1,    60,   335,
      -1,    61,   335,    -1,    62,   335,    -1,    63,   335,    -1,
      64,   335,    -1,    65,   335,    -1,    66,   335,    -1,    -1,
      84,   186,   236,   237,   187,    -1,    -1,   237,   238,    -1,
     100,   336,    -1,    48,   186,   240,   187,    -1,   137,   331,
      -1,    -1,   138,   188,   239,   284,   189,    -1,    -1,   240,
     241,    -1,    53,   335,    -1,    54,   335,    -1,    -1,    85,
     186,   243,   244,   187,    -1,    -1,   244,   245,    -1,   100,
     336,    -1,   121,   338,    -1,    -1,    86,   186,   247,   248,
     187,    -1,    -1,   248,   249,    -1,   100,   336,    -1,   250,
      -1,    -1,    87,   186,   251,   252,   187,    -1,    -1,   252,
     253,    -1,   100,   336,    -1,    -1,   173,   254,   255,    -1,
     137,   331,    -1,   286,    -1,   291,    -1,    85,   336,    -1,
      85,   340,    -1,   179,    -1,   180,    -1,    -1,    88,   186,
     257,   258,   187,    -1,    -1,   258,   259,    -1,   100,   336,
      -1,    86,   336,    -1,    86,   340,    -1,    86,    68,    -1,
     286,    -1,   291,    -1,   260,    -1,    -1,    89,   186,   261,
     262,   187,    -1,    -1,   262,   263,    -1,   100,   336,    -1,
      84,   336,    -1,    -1,   120,   188,   264,   265,   189,    -1,
     137,   331,    -1,   286,    -1,   291,    -1,    -1,   265,   266,
      -1,   134,    -1,   135,    -1,   136,    -1,    -1,    90,   186,
     268,   269,   187,    -1,    -1,   269,   270,    -1,   267,    -1,
     100,   336,    -1,    84,   336,    -1,    84,   274,    -1,    -1,
     119,   188,   271,   278,   189,    -1,    -1,   120,   188,   272,
     280,   189,    -1,    -1,   127,   188,   273,   282,   189,    -1,
     137,   331,    -1,   142,   188,   338,   338,   189,    -1,   143,
     340,    -1,   144,   340,    -1,   178,   188,   338,   338,   189,
      -1,   145,   188,   340,   340,   340,   340,   189,    -1,   146,
     188,   340,   340,   189,    -1,   286,    -1,   291,    -1,   295,
      -1,   299,    -1,   317,    -1,   321,    -1,   323,    -1,    48,
     186,   329,   187,    -1,    -1,   190,   275,   276,   191,    -1,
      -1,   276,   277,    -1,    88,   336,    -1,    88,   340,    -1,
      89,   336,    -1,    89,   340,    -1,    86,   336,    -1,    86,
     340,    -1,    87,   336,    -1,    87,   340,    -1,    -1,   278,
     279,    -1,   128,    -1,   129,    -1,   130,    -1,   131,    -1,
      -1,   280,   281,    -1,   134,    -1,   135,    -1,   136,    -1,
      -1,   282,   283,    -1,   132,    -1,   133,    -1,    -1,   284,
     285,    -1,   139,    -1,   140,    -1,   141,    -1,    -1,   149,
     186,   287,   288,   187,    -1,    -1,   288,   289,    -1,   150,
     188,   338,   338,   338,   189,    -1,   151,   188,   338,   338,
     338,   189,    -1,   152,   188,   338,   338,   338,   189,    -1,
     101,   290,    -1,   110,    -1,   111,    -1,    -1,   153,   186,
     292,   293,   187,    -1,    -1,   293,   294,    -1,   154,   188,
     338,   338,   338,   189,    -1,   155,   338,    -1,   156,   188,
     338,   338,   189,    -1,   157,   188,   338,   338,   338,   189,
      -1,    91,   186,   296,   187,    -1,    -1,   296,   297,    -1,
     173,   298,    -1,    98,   338,    -1,   175,   338,    -1,    82,
     338,    -1,   177,   188,   340,   340,   189,    -1,   177,   338,
      -1,   174,    -1,   176,    -1,    78,    -1,    79,    -1,    83,
      -1,    80,    -1,    81,    -1,   300,    -1,   302,    -1,   303,
      -1,   305,    -1,   307,    -1,   308,    -1,    -1,    92,   186,
     301,   309,   187,    -1,    93,   186,   187,    -1,    -1,    94,
     186,   304,   311,   187,    -1,    -1,    95,   186,   306,   314,
     187,    -1,    96,   186,   187,    -1,    97,   186,   187,    -1,
      -1,   309,   310,    -1,    98,   338,    -1,    82,   338,    -1,
      -1,   311,   312,    -1,    98,   338,    -1,   177,   188,   340,
     340,   189,    -1,   175,   338,    -1,   177,   338,    -1,   173,
     313,    -1,   174,    -1,   176,    -1,    78,    -1,    79,    -1,
      -1,   314,   315,    -1,    98,   338,    -1,   177,   188,   340,
     340,   189,    -1,   177,   338,    -1,   173,   316,    -1,   174,
      -1,   176,    -1,    78,    -1,    79,    -1,    -1,   159,   186,
     318,   319,   187,    -1,    -1,   319,   320,    -1,   100,   336,
      -1,   160,   335,    -1,   161,   335,    -1,    -1,   162,   186,
     322,   325,   187,    -1,    -1,   163,   186,   324,   325,   187,
      -1,    -1,   325,   326,    -1,   100,   336,    -1,   101,   328,
      -1,   137,   331,    -1,    -1,   127,   188,   327,   282,   189,
      -1,   178,   188,   338,   338,   189,    -1,   108,    -1,   109,
      -1,    -1,   329,   330,    -1,   164,   335,    -1,   165,   332,
      -1,   166,   332,    -1,   167,   335,    -1,   168,   335,    -1,
     188,   338,   338,   338,   338,   189,    -1,   188,   333,   189,
      -1,    -1,   334,    -1,   333,   334,    -1,    75,    -1,    76,
      -1,    77,    -1,    67,    -1,    68,    -1,    69,    -1,    70,
      -1,    78,    -1,    71,    -1,    72,    -1,    73,    -1,    74,
      -1,    79,    -1,    47,    -1,   170,    -1,   169,    -1,   124,
      -1,   125,    -1,   126,    -1,   171,    -1,   172,    -1,   339,
      -1,   181,    -1,   182,    -1,   183,    -1,   339,    -1,   184,
      -1,   340,    -1,   185,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   301,   301,   301,   303,   303,   309,   310,   313,   313,
     316,   321,   326,   331,   336,   341,   346,   351,   356,   361,
     366,   371,   376,   381,   386,   391,   396,   401,   406,   411,
     416,   421,   426,   431,   436,   441,   446,   451,   456,   461,
     466,   471,   476,   481,   486,   491,   498,   499,   500,   501,
     502,   503,   504,   506,   506,   510,   510,   512,   511,   523,
     523,   525,   524,   532,   532,   534,   535,   536,   537,   538,
     539,   540,   541,   542,   542,   544,   546,   549,   549,   550,
     550,   556,   555,   562,   562,   564,   565,   566,   567,   570,
     569,   576,   576,   578,   579,   580,   581,   582,   583,   585,
     585,   587,   589,   592,   594,   596,   605,   604,   610,   610,
     612,   613,   614,   615,   616,   617,   622,   622,   624,   626,
     630,   629,   636,   636,   638,   639,   640,   641,   649,   649,
     651,   653,   655,   657,   659,   661,   663,   665,   667,   669,
     671,   673,   675,   677,   679,   683,   682,   689,   689,   691,
     692,   694,   702,   702,   704,   704,   706,   709,   714,   714,
     716,   716,   718,   719,   721,   721,   723,   723,   725,   726,
     728,   728,   730,   730,   732,   733,   733,   735,   737,   738,
     739,   751,   765,   766,   768,   768,   770,   770,   772,   773,
     784,   796,   800,   801,   802,   804,   804,   806,   806,   808,
     809,   821,   821,   823,   825,   826,   828,   828,   830,   831,
     832,   835,   834,   844,   844,   846,   847,   848,   860,   885,
     885,   887,   887,   889,   889,   891,   893,   895,   896,   897,
     899,   901,   903,   904,   905,   906,   907,   908,   909,   910,
     913,   913,   921,   921,   923,   930,   937,   944,   951,   958,
     965,   972,   980,   980,   982,   983,   984,   985,   987,   987,
     989,   990,   991,   993,   993,   995,   996,   998,   998,  1000,
    1001,  1002,  1004,  1004,  1006,  1006,  1008,  1010,  1012,  1014,
    1017,  1018,  1020,  1020,  1023,  1023,  1025,  1027,  1029,  1031,
    1035,  1044,  1044,  1046,  1047,  1054,  1055,  1056,  1058,  1061,
    1067,  1073,  1079,  1085,  1089,  1094,  1099,  1099,  1099,  1099,
    1100,  1100,  1103,  1102,  1109,  1114,  1113,  1121,  1120,  1127,
    1131,  1136,  1136,  1138,  1139,  1141,  1141,  1143,  1144,  1146,
    1147,  1148,  1151,  1152,  1153,  1154,  1156,  1156,  1158,  1159,
    1161,  1162,  1165,  1166,  1167,  1168,  1171,  1171,  1177,  1177,
    1178,  1179,  1180,  1184,  1184,  1190,  1190,  1196,  1196,  1198,
    1199,  1200,  1202,  1202,  1204,  1208,  1209,  1211,  1211,  1213,
    1215,  1218,  1221,  1223,  1226,  1234,  1236,  1237,  1238,  1240,
    1241,  1242,  1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,
    1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,
    1263,  1265,  1274,  1276,  1277,  1279,  1280,  1281
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "EQTOKEN_HEADER", "EQTOKEN_ASCII",
  "EQTOKEN_GLOBAL", "EQTOKEN_CONNECTION_SATTR_HOSTNAME",
  "EQTOKEN_CONNECTION_IATTR_TYPE", "EQTOKEN_CONNECTION_IATTR_PORT",
  "EQTOKEN_CONNECTION_SATTR_FILENAME",
  "EQTOKEN_CONNECTION_IATTR_BANDWIDTH", "EQTOKEN_CONFIG_FATTR_EYE_BASE",
  "EQTOKEN_CONFIG_IATTR_ROBUSTNESS", "EQTOKEN_NODE_SATTR_LAUNCH_COMMAND",
  "EQTOKEN_NODE_CATTR_LAUNCH_COMMAND_QUOTE",
  "EQTOKEN_NODE_IATTR_THREAD_MODEL", "EQTOKEN_NODE_IATTR_LAUNCH_TIMEOUT",
  "EQTOKEN_NODE_IATTR_HINT_STATISTICS", "EQTOKEN_PIPE_IATTR_HINT_THREAD",
  "EQTOKEN_PIPE_IATTR_HINT_CUDA_GL_INTEROP",
  "EQTOKEN_WINDOW_IATTR_HINT_STEREO",
  "EQTOKEN_WINDOW_IATTR_HINT_DOUBLEBUFFER",
  "EQTOKEN_WINDOW_IATTR_HINT_FULLSCREEN",
  "EQTOKEN_WINDOW_IATTR_HINT_DECORATION",
  "EQTOKEN_WINDOW_IATTR_HINT_SWAPSYNC",
  "EQTOKEN_WINDOW_IATTR_HINT_DRAWABLE",
  "EQTOKEN_WINDOW_IATTR_HINT_STATISTICS",
  "EQTOKEN_WINDOW_IATTR_HINT_SCREENSAVER",
  "EQTOKEN_WINDOW_IATTR_PLANES_COLOR", "EQTOKEN_WINDOW_IATTR_PLANES_ALPHA",
  "EQTOKEN_WINDOW_IATTR_PLANES_DEPTH",
  "EQTOKEN_WINDOW_IATTR_PLANES_STENCIL",
  "EQTOKEN_WINDOW_IATTR_PLANES_ACCUM",
  "EQTOKEN_WINDOW_IATTR_PLANES_ACCUM_ALPHA",
  "EQTOKEN_WINDOW_IATTR_PLANES_SAMPLES",
  "EQTOKEN_COMPOUND_IATTR_STEREO_MODE",
  "EQTOKEN_COMPOUND_IATTR_STEREO_ANAGLYPH_LEFT_MASK",
  "EQTOKEN_COMPOUND_IATTR_STEREO_ANAGLYPH_RIGHT_MASK",
  "EQTOKEN_COMPOUND_IATTR_UPDATE_FOV",
  "EQTOKEN_COMPOUND_IATTR_HINT_OFFSET",
  "EQTOKEN_CHANNEL_IATTR_HINT_STATISTICS",
  "EQTOKEN_CHANNEL_IATTR_HINT_SENDTOKEN", "EQTOKEN_SERVER",
  "EQTOKEN_CONFIG", "EQTOKEN_APPNODE", "EQTOKEN_NODE", "EQTOKEN_PIPE",
  "EQTOKEN_WINDOW", "EQTOKEN_ATTRIBUTES", "EQTOKEN_HINT_STEREO",
  "EQTOKEN_HINT_DOUBLEBUFFER", "EQTOKEN_HINT_FULLSCREEN",
  "EQTOKEN_HINT_DECORATION", "EQTOKEN_HINT_STATISTICS",
  "EQTOKEN_HINT_SENDTOKEN", "EQTOKEN_HINT_SWAPSYNC",
  "EQTOKEN_HINT_DRAWABLE", "EQTOKEN_HINT_THREAD",
  "EQTOKEN_HINT_CUDA_GL_INTEROP", "EQTOKEN_HINT_SCREENSAVER",
  "EQTOKEN_PLANES_COLOR", "EQTOKEN_PLANES_ALPHA", "EQTOKEN_PLANES_DEPTH",
  "EQTOKEN_PLANES_STENCIL", "EQTOKEN_PLANES_ACCUM",
  "EQTOKEN_PLANES_ACCUM_ALPHA", "EQTOKEN_PLANES_SAMPLES", "EQTOKEN_ON",
  "EQTOKEN_OFF", "EQTOKEN_AUTO", "EQTOKEN_FASTEST", "EQTOKEN_NICEST",
  "EQTOKEN_QUAD", "EQTOKEN_ANAGLYPH", "EQTOKEN_PASSIVE", "EQTOKEN_RED",
  "EQTOKEN_GREEN", "EQTOKEN_BLUE", "EQTOKEN_HORIZONTAL",
  "EQTOKEN_VERTICAL", "EQTOKEN_DFR", "EQTOKEN_DDS", "EQTOKEN_FRAMERATE",
  "EQTOKEN_DPLEX", "EQTOKEN_CHANNEL", "EQTOKEN_OBSERVER", "EQTOKEN_LAYOUT",
  "EQTOKEN_VIEW", "EQTOKEN_CANVAS", "EQTOKEN_SEGMENT", "EQTOKEN_COMPOUND",
  "EQTOKEN_LOADBALANCER", "EQTOKEN_DFREQUALIZER",
  "EQTOKEN_FRAMERATEEQUALIZER", "EQTOKEN_LOADEQUALIZER",
  "EQTOKEN_TREEEQUALIZER", "EQTOKEN_MONITOREQUALIZER",
  "EQTOKEN_VIEWEQUALIZER", "EQTOKEN_DAMPING", "EQTOKEN_CONNECTION",
  "EQTOKEN_NAME", "EQTOKEN_TYPE", "EQTOKEN_TCPIP", "EQTOKEN_SDP",
  "EQTOKEN_IB", "EQTOKEN_MCIP", "EQTOKEN_PGM", "EQTOKEN_RSP",
  "EQTOKEN_TEXTURE", "EQTOKEN_MEMORY", "EQTOKEN_FIXED", "EQTOKEN_HMD",
  "EQTOKEN_HOSTNAME", "EQTOKEN_INTERFACE", "EQTOKEN_LAUNCH_COMMAND",
  "EQTOKEN_LAUNCH_COMMAND_QUOTE", "EQTOKEN_LAUNCH_TIMEOUT", "EQTOKEN_PORT",
  "EQTOKEN_FILENAME", "EQTOKEN_TASK", "EQTOKEN_EYE", "EQTOKEN_EYE_BASE",
  "EQTOKEN_ROBUSTNESS", "EQTOKEN_THREAD_MODEL", "EQTOKEN_ASYNC",
  "EQTOKEN_DRAW_SYNC", "EQTOKEN_LOCAL_SYNC", "EQTOKEN_BUFFER",
  "EQTOKEN_CLEAR", "EQTOKEN_DRAW", "EQTOKEN_ASSEMBLE", "EQTOKEN_READBACK",
  "EQTOKEN_COLOR", "EQTOKEN_DEPTH", "EQTOKEN_CYCLOP", "EQTOKEN_LEFT",
  "EQTOKEN_RIGHT", "EQTOKEN_VIEWPORT", "EQTOKEN_DRAWABLE",
  "EQTOKEN_FBO_COLOR", "EQTOKEN_FBO_DEPTH", "EQTOKEN_FBO_STENCIL",
  "EQTOKEN_RANGE", "EQTOKEN_PERIOD", "EQTOKEN_PHASE", "EQTOKEN_PIXEL",
  "EQTOKEN_SUBPIXEL", "EQTOKEN_BANDWIDTH", "EQTOKEN_DEVICE",
  "EQTOKEN_WALL", "EQTOKEN_BOTTOM_LEFT", "EQTOKEN_BOTTOM_RIGHT",
  "EQTOKEN_TOP_LEFT", "EQTOKEN_PROJECTION", "EQTOKEN_ORIGIN",
  "EQTOKEN_DISTANCE", "EQTOKEN_FOV", "EQTOKEN_HPR", "EQTOKEN_LATENCY",
  "EQTOKEN_SWAPBARRIER", "EQTOKEN_NVGROUP", "EQTOKEN_NVBARRIER",
  "EQTOKEN_OUTPUTFRAME", "EQTOKEN_INPUTFRAME", "EQTOKEN_STEREO_MODE",
  "EQTOKEN_STEREO_ANAGLYPH_LEFT_MASK",
  "EQTOKEN_STEREO_ANAGLYPH_RIGHT_MASK", "EQTOKEN_UPDATE_FOV",
  "EQTOKEN_HINT_OFFSET", "EQTOKEN_PBUFFER", "EQTOKEN_FBO",
  "EQTOKEN_RGBA16F", "EQTOKEN_RGBA32F", "EQTOKEN_MODE", "EQTOKEN_2D",
  "EQTOKEN_ASSEMBLE_ONLY_LIMIT", "EQTOKEN_DB", "EQTOKEN_BOUNDARY",
  "EQTOKEN_ZOOM", "EQTOKEN_MONO", "EQTOKEN_STEREO", "EQTOKEN_STRING",
  "EQTOKEN_CHARACTER", "EQTOKEN_FLOAT", "EQTOKEN_INTEGER",
  "EQTOKEN_UNSIGNED", "'{'", "'}'", "'['", "']'", "'('", "')'", "$accept",
  "file", "header", "global", "globals", "connectionType", "server", "$@1",
  "serverConnections", "serverConnection", "$@2", "configs", "config",
  "$@3", "configFields", "configField", "configAttributes",
  "configAttribute", "node", "renderNode", "$@4", "appNode", "$@5",
  "nodeFields", "nodeField", "connection", "$@6", "connectionFields",
  "connectionField", "nodeAttributes", "nodeAttribute", "pipe", "$@7",
  "pipeFields", "pipeField", "pipeAttributes", "pipeAttribute", "window",
  "$@8", "windowFields", "windowField", "windowAttributes",
  "windowAttribute", "channel", "$@9", "channelFields", "channelField",
  "$@10", "channelAttributes", "channelAttribute", "observer", "$@11",
  "observerFields", "observerField", "layout", "$@12", "layoutFields",
  "layoutField", "view", "$@13", "viewFields", "viewField", "$@14",
  "viewMode", "canvas", "$@15", "canvasFields", "canvasField", "segment",
  "$@16", "segmentFields", "segmentField", "$@17", "segumentEyes",
  "segumentEye", "compound", "$@18", "compoundFields", "compoundField",
  "$@19", "$@20", "$@21", "viewSegmentRef", "$@22", "viewSegmentRefFields",
  "viewSegmentRefField", "compoundTasks", "compoundTask", "compoundEyes",
  "compoundEye", "buffers", "buffer", "drawables", "drawable", "wall",
  "$@23", "wallFields", "wallField", "wallType", "projection", "$@24",
  "projectionFields", "projectionField", "loadBalancer",
  "loadBalancerFields", "loadBalancerField", "loadBalancerMode",
  "equalizer", "dfrEqualizer", "$@25", "framerateEqualizer",
  "loadEqualizer", "$@26", "treeEqualizer", "$@27", "monitorEqualizer",
  "viewEqualizer", "dfrEqualizerFields", "dfrEqualizerField",
  "loadEqualizerFields", "loadEqualizerField", "loadEqualizerMode",
  "treeEqualizerFields", "treeEqualizerField", "treeEqualizerMode",
  "swapBarrier", "$@28", "swapBarrierFields", "swapBarrierField",
  "outputFrame", "$@29", "inputFrame", "$@30", "frameFields", "frameField",
  "$@31", "frameType", "compoundAttributes", "compoundAttribute",
  "viewport", "colorMask", "colorMaskBits", "colorMaskBit", "IATTR",
  "STRING", "CHARACTER", "FLOAT", "INTEGER", "UNSIGNED", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   123,   125,    91,    93,
      40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   192,   193,   193,   194,   194,   195,   195,   196,   196,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   197,   197,   197,   197,
     197,   197,   197,   199,   198,   200,   200,   202,   201,   203,
     203,   205,   204,   206,   206,   207,   207,   207,   207,   207,
     207,   207,   207,   208,   208,   209,   209,   210,   210,   212,
     211,   214,   213,   215,   215,   216,   216,   216,   216,   218,
     217,   219,   219,   220,   220,   220,   220,   220,   220,   221,
     221,   222,   222,   222,   222,   222,   224,   223,   225,   225,
     226,   226,   226,   226,   226,   226,   227,   227,   228,   228,
     230,   229,   231,   231,   232,   232,   232,   232,   233,   233,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   236,   235,   237,   237,   238,
     238,   238,   239,   238,   240,   240,   241,   241,   243,   242,
     244,   244,   245,   245,   247,   246,   248,   248,   249,   249,
     251,   250,   252,   252,   253,   254,   253,   253,   253,   253,
     253,   253,   255,   255,   257,   256,   258,   258,   259,   259,
     259,   259,   259,   259,   259,   261,   260,   262,   262,   263,
     263,   264,   263,   263,   263,   263,   265,   265,   266,   266,
     266,   268,   267,   269,   269,   270,   270,   270,   270,   271,
     270,   272,   270,   273,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     275,   274,   276,   276,   277,   277,   277,   277,   277,   277,
     277,   277,   278,   278,   279,   279,   279,   279,   280,   280,
     281,   281,   281,   282,   282,   283,   283,   284,   284,   285,
     285,   285,   287,   286,   288,   288,   289,   289,   289,   289,
     290,   290,   292,   291,   293,   293,   294,   294,   294,   294,
     295,   296,   296,   297,   297,   297,   297,   297,   297,   298,
     298,   298,   298,   298,   298,   298,   299,   299,   299,   299,
     299,   299,   301,   300,   302,   304,   303,   306,   305,   307,
     308,   309,   309,   310,   310,   311,   311,   312,   312,   312,
     312,   312,   313,   313,   313,   313,   314,   314,   315,   315,
     315,   315,   316,   316,   316,   316,   318,   317,   319,   319,
     320,   320,   320,   322,   321,   324,   323,   325,   325,   326,
     326,   326,   327,   326,   326,   328,   328,   329,   329,   330,
     330,   330,   330,   330,   331,   332,   333,   333,   333,   334,
     334,   334,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   336,   337,   338,   338,   339,   339,   340
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     0,     3,     4,     0,     0,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     0,     6,     0,     2,     0,     5,     1,
       2,     0,     5,     0,     2,     1,     2,     1,     1,     1,
       1,     2,     4,     0,     2,     2,     2,     1,     1,     0,
       5,     0,     5,     0,     2,     2,     1,     1,     4,     0,
       5,     0,     2,     2,     2,     2,     2,     2,     2,     0,
       2,     2,     2,     2,     2,     2,     0,     5,     0,     2,
       1,     4,     2,     2,     2,     2,     0,     2,     2,     2,
       0,     5,     0,     2,     1,     4,     2,     2,     0,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     5,     0,     2,     2,
       4,     2,     0,     5,     0,     2,     2,     2,     0,     5,
       0,     2,     2,     2,     0,     5,     0,     2,     2,     1,
       0,     5,     0,     2,     2,     0,     3,     2,     1,     1,
       2,     2,     1,     1,     0,     5,     0,     2,     2,     2,
       2,     2,     1,     1,     1,     0,     5,     0,     2,     2,
       2,     0,     5,     2,     1,     1,     0,     2,     1,     1,
       1,     0,     5,     0,     2,     1,     2,     2,     2,     0,
       5,     0,     5,     0,     5,     2,     5,     2,     2,     5,
       7,     5,     1,     1,     1,     1,     1,     1,     1,     4,
       0,     4,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     0,     2,     1,     1,     1,     1,     0,     2,
       1,     1,     1,     0,     2,     1,     1,     0,     2,     1,
       1,     1,     0,     5,     0,     2,     6,     6,     6,     2,
       1,     1,     0,     5,     0,     2,     6,     2,     5,     6,
       4,     0,     2,     2,     2,     2,     2,     5,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     5,     3,     0,     5,     0,     5,     3,
       3,     0,     2,     2,     2,     0,     2,     2,     5,     2,
       2,     2,     1,     1,     1,     1,     0,     2,     2,     5,
       2,     2,     1,     1,     1,     1,     0,     5,     0,     2,
       2,     2,     2,     0,     5,     0,     5,     0,     2,     2,
       2,     2,     0,     5,     5,     1,     1,     0,     2,     2,
       2,     2,     2,     2,     6,     3,     0,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     7,   403,   405,   407,     0,   404,   406,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       8,   401,    10,    49,    46,    47,    48,    50,    51,    52,
      11,    12,    13,    14,    15,   392,   382,   383,   384,   385,
     387,   388,   389,   390,   386,   391,   395,   396,   397,   394,
     393,   398,   399,    16,   400,    17,   402,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    41,   376,
      42,    43,    44,    45,    39,    40,     0,     0,     2,     3,
       0,   379,   380,   381,     0,   377,    53,    61,     6,     9,
     375,   378,    55,    63,     0,     0,     0,    56,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      64,    65,    78,    77,    67,    68,    69,    70,    57,    54,
      60,    81,    79,    73,   158,   164,   184,   211,    66,    71,
      91,    83,    83,     0,   160,   166,   186,   213,     0,     0,
       0,     0,     0,    72,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    92,     0,     0,     0,
       0,    82,    84,    86,    87,    80,    75,    76,     0,     0,
     159,   161,     0,     0,   165,   167,   169,     0,     0,     0,
       0,     0,   185,   187,   194,   192,   193,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     212,   215,   214,   232,   233,   234,   235,   306,   307,   308,
     309,   310,   311,   236,   237,   238,    93,    94,    95,    96,
      98,    97,   106,    99,    89,    85,   162,   163,   170,   168,
     191,   189,   190,   195,   188,   272,   282,   367,   240,   218,
     217,   291,   312,     0,   315,   317,     0,     0,   216,   219,
     221,   223,     0,   225,     0,   227,   228,     0,     0,   346,
     353,   355,     0,   108,     0,    91,   172,   197,   274,   284,
       0,   242,     0,   321,   314,   325,   336,   319,   320,   252,
     258,   263,     0,     0,     0,     0,   348,   357,   357,     0,
       0,     0,     0,     0,     0,     0,    88,   100,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,   368,
       0,     0,     0,     0,     0,     0,   290,   292,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   107,   109,
     110,   105,   101,   102,   104,   103,    90,     0,     0,     0,
     175,   171,   173,   178,   179,     0,     0,     0,     0,   196,
     198,   204,   205,     0,     0,     0,     0,   273,   275,     0,
       0,     0,     0,   283,   285,   369,   370,   371,   372,   373,
       0,     0,     0,     0,   241,   243,   296,   294,   301,   302,
     304,   305,   303,   299,   300,   293,   295,     0,   298,     0,
       0,   313,   322,     0,     0,     0,     0,   316,   326,     0,
       0,     0,   318,   337,   254,   255,   256,   257,   220,   253,
     260,   261,   262,   222,   259,   265,   266,   224,   264,     0,
     226,     0,   231,     0,     0,     0,   347,   349,     0,     0,
       0,     0,     0,   354,   358,   356,   229,   120,   116,   112,
     113,   115,   114,   180,   181,   174,   177,     0,   200,   199,
     201,   203,   280,   281,   279,     0,     0,     0,     0,   287,
       0,     0,   248,   249,   250,   251,   244,   245,   246,   247,
       0,   324,   323,   327,   334,   335,   332,   333,   331,   329,
       0,   330,   338,   344,   345,   342,   343,   341,     0,   340,
       0,     0,   350,   351,   352,   359,   365,   366,   360,   362,
     361,     0,   122,     0,   182,   183,   176,   206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   374,   230,   263,
       0,     0,     0,     0,   111,   117,     0,     0,     0,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,   121,   123,   124,   118,   119,   208,   209,   210,
     202,   207,     0,     0,     0,     0,   288,     0,   328,   339,
     363,   364,   128,   145,   126,   127,   276,   277,   278,   286,
     289,     0,   147,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   125,   129,
       0,   130,   131,   132,   133,   136,   134,   135,   137,   138,
     139,   140,   141,   142,   143,   144,     0,     0,     0,     0,
     146,   148,   154,   149,   151,   152,     0,   267,     0,     0,
     150,   155,     0,   156,   157,   269,   270,   271,   153,   268
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    48,   120,    60,   118,   132,   134,   137,
     170,   138,   119,   133,   135,   150,   173,   184,   151,   152,
     172,   153,   171,   179,   202,   203,   315,   178,   196,   314,
     347,   204,   313,   340,   389,   563,   585,   390,   562,   581,
     603,   631,   649,   604,   632,   650,   671,   677,   676,   681,
     154,   174,   185,   211,   155,   175,   186,   215,   216,   316,
     349,   402,   507,   566,   156,   176,   187,   223,   224,   317,
     350,   410,   567,   586,   611,   157,   177,   188,   252,   329,
     330,   331,   289,   321,   360,   435,   371,   469,   372,   474,
     373,   478,   682,   689,   225,   318,   351,   418,   514,   226,
     319,   352,   424,   255,   322,   367,   445,   256,   257,   323,
     258,   259,   325,   260,   326,   261,   262,   368,   452,   369,
     458,   538,   370,   463,   547,   263,   336,   378,   487,   264,
     337,   265,   338,   379,   494,   579,   558,   320,   359,   303,
     110,   124,   125,    83,    52,    87,     7,    84,     9
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -366
static const yytype_int16 yypact[] =
{
      61,   -28,    69,   932,  -366,  -366,  -366,    68,  -366,  -366,
    -366,  -110,  -102,   385,  -101,  -102,  -101,   -28,   329,  -102,
     -86,   329,  -101,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   -79,   -79,   329,   329,   329,   329,    45,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,   295,
    -366,  -366,  -366,  -366,  -366,  -366,   -74,   -63,  -366,  -366,
     170,  -366,  -366,  -366,   -36,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,    25,   -15,   -56,  -366,   -16,  -366,
     -53,   -23,   -13,    40,    47,    51,    57,  -102,  -101,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,   -59,  -366,  -366,  -366,  -366,   309,    32,
      35,   -28,   329,  -366,  -366,   139,   -65,   -39,   202,   385,
    -102,  -102,  -101,  -102,  -101,  -366,  -366,    62,    66,    67,
    -102,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -102,   -28,
    -366,  -366,    75,  -102,  -366,  -366,  -366,   -40,    76,  -102,
      81,    88,  -366,  -366,  -366,  -366,  -366,    90,  -136,    91,
      93,    97,   102,   104,   105,   115,  -102,   -61,   -30,    96,
     116,   122,  -101,  -101,   123,   127,   130,   144,   145,   146,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,   140,  -366,  -366,   151,   153,  -366,  -366,
    -366,  -366,   -28,  -366,   -28,  -366,  -366,  -101,  -101,  -366,
    -366,  -366,   -28,  -366,    36,  -366,  -366,  -366,  -366,  -366,
     352,  -366,   -33,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,   -28,   -28,  -101,  -101,  -366,  -366,  -366,   -28,
     121,   329,  -102,   -86,   329,   329,  -366,  -366,   361,   200,
     205,   278,   348,   329,   -79,   -79,   329,   329,  -366,  -366,
     -31,   -28,   -28,   192,   -28,   -67,  -366,  -366,   -38,    72,
     -62,   320,   247,    92,   -28,   120,  -101,   152,   -41,   141,
     206,   161,   166,   173,  -102,  -101,   116,  -101,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -133,  -102,   116,
    -366,  -366,  -366,  -366,  -366,  -102,  -102,   168,   116,  -366,
    -366,  -366,  -366,   -11,   172,   174,   175,  -366,  -366,   186,
     -28,   190,   198,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -133,  -133,  -133,  -133,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -101,  -366,   -28,
     -28,  -366,  -366,   -28,   -12,   -28,   256,  -366,  -366,   -28,
      58,   297,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,   -28,
    -366,  -101,  -366,  -102,   329,   329,  -366,  -366,  -102,    30,
     203,   116,   207,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,    77,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,   -28,   -28,   -28,   -28,  -366,
     -28,   -28,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -101,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -101,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -101,  -366,
     199,   201,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,   -28,  -366,   -20,  -366,  -366,  -366,  -366,   -28,   -28,
     -28,   -28,   -28,   -28,   215,  -101,  -101,  -366,  -366,  -366,
     -28,    29,   329,   329,  -366,  -366,   283,   -28,   -28,   -28,
     -28,   220,   -28,  -366,   224,   225,    98,   227,   208,   237,
    -102,   116,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,   231,   253,   257,   263,  -366,   272,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,    42,  -366,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,  -366,  -366,
     180,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,   259,  -102,   116,   279,
    -366,  -366,  -366,  -366,  -366,  -366,   -22,  -366,   329,   329,
    -366,  -366,   125,  -366,  -366,  -366,  -366,  -366,  -366,  -366
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -366,  -366,  -366,   346,  -366,   281,  -366,  -366,  -366,  -366,
    -366,  -366,   -52,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,   -25,  -366,  -366,  -366,   160,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,   280,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -103,  -366,  -366,  -366,  -137,  -366,  -366,  -366,  -366,  -135,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,  -366,
    -366,  -366,  -366,   148,  -366,  -366,  -366,  -366,  -366,  -365,
     -42,  -366,   359,   -21,    27,   154,   234,    73,   455
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      88,   111,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   501,   212,   112,   113,   114,   115,   117,   280,   140,
     141,   678,   679,   142,   506,   213,   459,   582,   583,   121,
     122,   123,    62,   511,   449,    51,    85,   217,    51,   361,
     218,   253,     6,   254,   288,   430,   431,   432,   433,   483,
     450,   219,   181,   182,     1,   362,   534,   535,   117,    10,
     143,   144,    49,   145,     8,   146,    50,   598,   197,    51,
     198,   197,   139,   198,     6,   147,   160,   116,   117,   341,
       8,   633,   634,   635,   636,   637,    86,   638,   639,   512,
     513,   640,   641,   642,   643,   644,   645,   646,   647,   109,
     220,   460,   126,   599,   221,   461,     4,     5,     6,   484,
     485,   447,   214,   127,   136,   462,   560,   299,   183,   600,
     158,   199,   200,   161,   199,   200,   543,   544,   556,   557,
     363,    51,   364,   148,   365,     6,   486,   180,   222,   451,
     342,   343,   344,   130,   366,     4,     5,     6,   300,   345,
     434,   207,   536,   162,   537,   680,   601,   584,   382,   383,
     453,   159,   149,   163,   168,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,   403,   411,   404,   412,   602,   267,   268,   201,
     270,   384,   205,   346,   475,   476,   164,   275,   666,   648,
     475,   476,   545,   165,   546,   276,   625,   166,   385,   208,
     279,   488,   489,   167,   281,   454,   284,   455,   272,   456,
     227,    64,   273,   274,     8,   290,   564,   565,   386,   457,
     209,   278,   283,   298,   685,   686,   687,   285,   490,   387,
     438,   439,   440,   441,   286,   442,   287,   291,   491,   292,
     667,   477,     8,   293,   301,   397,   228,   620,   294,   405,
     295,   296,   146,   229,   230,   231,   232,   233,   234,   235,
     398,   297,   236,   674,   302,   406,   488,   489,   388,   480,
     304,   307,   426,   427,   688,   308,   309,   668,   669,   492,
     391,   237,   238,   394,   395,   407,   210,   324,   493,   239,
     310,   311,   425,   490,   312,   428,   429,   399,   327,   240,
     328,   482,   408,   491,   241,   242,   243,   244,   245,   220,
     496,   220,   497,   221,   220,   221,   510,   128,   221,   498,
     515,   246,   516,   517,   247,   248,   443,   670,   444,   392,
     121,   122,   123,   400,   518,     8,    65,     8,   520,   413,
     249,   470,   471,   472,   492,     8,   521,   401,   577,   250,
     578,   559,   409,   495,   622,   561,    66,    67,    68,    69,
      70,    71,    72,    73,   593,     8,     8,    74,    75,   616,
     189,   499,     8,   618,   619,   206,   621,   607,   608,   609,
     626,   190,   191,   623,   503,   505,   192,   193,   414,   415,
     416,    53,   508,   509,     8,     8,   473,     8,     8,     4,
       5,     6,   627,   277,   540,   672,   628,     8,   464,   465,
     466,   467,   629,    76,    77,    78,   194,   522,   524,   526,
     528,   630,   189,   553,   554,   417,   129,   675,   251,    61,
     266,    63,   610,   190,   191,   348,   596,    89,   192,   193,
       4,     5,     6,   131,     0,   548,   380,    54,    55,    56,
      57,    58,    59,     8,     0,     0,   195,   393,    79,    80,
      81,    82,   419,   420,   421,   422,     0,     0,   194,   468,
     552,     0,     0,     5,     6,   555,   353,   354,   355,   356,
     357,     0,     8,     8,     0,     0,     8,     0,     8,     8,
       0,     0,     8,     0,     8,   423,   332,     0,   333,   358,
       0,     0,     0,     0,     0,     0,   339,     0,   396,     0,
       0,     0,     8,     0,     0,     0,     0,     0,     0,     0,
       0,   605,   606,     0,     0,     0,   374,   375,     0,     0,
       0,     0,     0,   381,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     8,     8,
       8,     8,     0,     8,     8,   436,   437,     0,   446,   448,
       0,     0,     0,   169,     0,     0,     0,     0,   479,     0,
       0,     0,   651,   652,   653,   654,   655,   656,   657,   658,
     659,   660,   661,   662,   663,   664,   665,   624,     0,     0,
       0,     0,     0,     0,     8,     0,     0,     0,     0,     0,
       0,     8,     8,     8,     8,     8,     8,   269,     0,   271,
       0,     0,     0,     8,   519,     0,     0,   683,   684,     0,
       8,     8,     8,     8,     0,     8,     0,     0,     0,     0,
       0,     0,   282,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   531,   532,     0,     0,   533,     0,   539,
     541,     0,     0,   542,   673,   549,     0,   305,   306,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   568,
     569,   570,   571,     0,   572,   573,     0,     0,     0,     0,
       0,     0,   334,   335,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   376,
     377,     0,     0,     0,     0,   580,     0,     0,     0,     0,
       0,     0,   587,   588,   589,   590,   591,   592,     0,     0,
       0,     0,     0,     0,   597,     0,     0,     0,     0,     0,
       0,   612,   613,   614,   615,     0,   617,     0,     0,     0,
       0,   481,     0,     0,     0,     0,     0,     0,     0,     0,
     500,     0,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   525,   527,   529,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   530,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   551,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   574,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   575,     0,     0,     0,     0,
       0,     0,     0,   576,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     594,   595
};

static const yytype_int16 yycheck[] =
{
      21,    43,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,   386,    87,    44,    45,    46,    47,    43,    68,    44,
      45,    53,    54,    48,   399,   100,    98,    57,    58,    75,
      76,    77,    15,   408,    82,   181,    19,    86,   181,    82,
      89,   188,   185,   188,   190,    86,    87,    88,    89,   100,
      98,   100,   121,   122,     3,    98,    78,    79,    43,     0,
      85,    86,     4,    88,     1,    90,   186,    48,    46,   181,
      48,    46,   134,    48,   185,   100,   138,    42,    43,    53,
      17,    49,    50,    51,    52,    53,   182,    55,    56,   110,
     111,    59,    60,    61,    62,    63,    64,    65,    66,   188,
     149,   173,   186,    84,   153,   177,   183,   184,   185,   160,
     161,   188,   187,   186,    99,   187,   491,   188,   187,   100,
     186,    99,   100,   186,    99,   100,    78,    79,   108,   109,
     173,   181,   175,   158,   177,   185,   187,   172,   187,   187,
     114,   115,   116,   189,   187,   183,   184,   185,   188,   123,
     191,   182,   174,   186,   176,   187,   137,   187,    47,    48,
      98,   187,   187,   186,   147,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,   349,   350,   349,   350,   187,   190,   191,   187,
     193,   100,   187,   187,   132,   133,   186,   200,    48,   187,
     132,   133,   174,   186,   176,   208,   601,   186,   117,   100,
     213,   100,   101,   186,   217,   173,   219,   175,   186,   177,
      48,    17,   186,   186,   181,   228,   179,   180,   137,   187,
     121,   186,   186,   236,   139,   140,   141,   186,   127,   148,
      78,    79,    80,    81,   186,    83,   186,   186,   137,   186,
     100,   189,   209,   186,   188,    85,    84,   189,   186,    84,
     186,   186,    90,    91,    92,    93,    94,    95,    96,    97,
     100,   186,   100,   668,   188,   100,   100,   101,   187,   189,
     188,   188,   354,   355,   189,   188,   186,   137,   138,   178,
     341,   119,   120,   344,   345,   120,   187,   187,   187,   127,
     186,   186,   353,   127,   188,   356,   357,   137,   187,   137,
     187,   189,   137,   137,   142,   143,   144,   145,   146,   149,
     189,   149,   186,   153,   149,   153,   188,   187,   153,   186,
     188,   159,   188,   188,   162,   163,   174,   187,   176,   342,
      75,    76,    77,   173,   188,   302,    47,   304,   188,   101,
     178,   134,   135,   136,   178,   312,   188,   187,   189,   187,
     189,   188,   187,   187,   186,   188,    67,    68,    69,    70,
      71,    72,    73,    74,   189,   332,   333,    78,    79,   189,
     101,   384,   339,   189,   189,   181,   189,   134,   135,   136,
     189,   112,   113,   186,   397,   398,   117,   118,   150,   151,
     152,    46,   405,   406,   361,   362,   189,   364,   365,   183,
     184,   185,   189,   209,   188,   186,   189,   374,   128,   129,
     130,   131,   189,   124,   125,   126,   147,   430,   431,   432,
     433,   189,   101,   484,   485,   187,   120,   188,   188,    14,
     189,    16,   189,   112,   113,   315,   579,    22,   117,   118,
     183,   184,   185,   124,    -1,   188,   338,   102,   103,   104,
     105,   106,   107,   420,    -1,    -1,   187,   343,   169,   170,
     171,   172,   154,   155,   156,   157,    -1,    -1,   147,   189,
     483,    -1,    -1,   184,   185,   488,   164,   165,   166,   167,
     168,    -1,   449,   450,    -1,    -1,   453,    -1,   455,   456,
      -1,    -1,   459,    -1,   461,   187,   302,    -1,   304,   187,
      -1,    -1,    -1,    -1,    -1,    -1,   312,    -1,   187,    -1,
      -1,    -1,   479,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   582,   583,    -1,    -1,    -1,   332,   333,    -1,    -1,
      -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   515,   516,
     517,   518,    -1,   520,   521,   361,   362,    -1,   364,   365,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   374,    -1,
      -1,    -1,   633,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,   647,   600,    -1,    -1,
      -1,    -1,    -1,    -1,   561,    -1,    -1,    -1,    -1,    -1,
      -1,   568,   569,   570,   571,   572,   573,   192,    -1,   194,
      -1,    -1,    -1,   580,   420,    -1,    -1,   678,   679,    -1,
     587,   588,   589,   590,    -1,   592,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   449,   450,    -1,    -1,   453,    -1,   455,
     456,    -1,    -1,   459,   667,   461,    -1,   242,   243,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   479,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   515,
     516,   517,   518,    -1,   520,   521,    -1,    -1,    -1,    -1,
      -1,    -1,   307,   308,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   334,
     335,    -1,    -1,    -1,    -1,   561,    -1,    -1,    -1,    -1,
      -1,    -1,   568,   569,   570,   571,   572,   573,    -1,    -1,
      -1,    -1,    -1,    -1,   580,    -1,    -1,    -1,    -1,    -1,
      -1,   587,   588,   589,   590,    -1,   592,    -1,    -1,    -1,
      -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     385,    -1,   387,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   430,   431,   432,   433,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   447,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   481,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   530,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   540,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   548,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     575,   576
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,   193,   194,   183,   184,   185,   338,   339,   340,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,   195,     4,
     186,   181,   336,    46,   102,   103,   104,   105,   106,   107,
     197,   340,   336,   340,   338,    47,    67,    68,    69,    70,
      71,    72,    73,    74,    78,    79,   124,   125,   126,   169,
     170,   171,   172,   335,   339,   336,   182,   337,   335,   340,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   188,
     332,   332,   335,   335,   335,   335,    42,    43,   198,   204,
     196,    75,    76,    77,   333,   334,   186,   186,   187,   195,
     189,   334,   199,   205,   200,   206,    99,   201,   203,   204,
      44,    45,    48,    85,    86,    88,    90,   100,   158,   187,
     207,   210,   211,   213,   242,   246,   256,   267,   186,   187,
     204,   186,   186,   186,   186,   186,   186,   186,   336,   340,
     202,   214,   212,   208,   243,   247,   257,   268,   219,   215,
     215,   121,   122,   187,   209,   244,   248,   258,   269,   101,
     112,   113,   117,   118,   147,   187,   220,    46,    48,    99,
     100,   187,   216,   217,   223,   187,   338,   335,   100,   121,
     187,   245,    87,   100,   187,   249,   250,    86,    89,   100,
     149,   153,   187,   259,   260,   286,   291,    48,    84,    91,
      92,    93,    94,    95,    96,    97,   100,   119,   120,   127,
     137,   142,   143,   144,   145,   146,   159,   162,   163,   178,
     187,   267,   270,   286,   291,   295,   299,   300,   302,   303,
     305,   307,   308,   317,   321,   323,   197,   336,   336,   340,
     336,   340,   186,   186,   186,   336,   336,   338,   186,   336,
      68,   336,   340,   186,   336,   186,   186,   186,   190,   274,
     336,   186,   186,   186,   186,   186,   186,   186,   336,   188,
     188,   188,   188,   331,   188,   340,   340,   188,   188,   186,
     186,   186,   188,   224,   221,   218,   251,   261,   287,   292,
     329,   275,   296,   301,   187,   304,   306,   187,   187,   271,
     272,   273,   338,   338,   340,   340,   318,   322,   324,   338,
     225,    53,   114,   115,   116,   123,   187,   222,   219,   252,
     262,   288,   293,   164,   165,   166,   167,   168,   187,   330,
     276,    82,    98,   173,   175,   177,   187,   297,   309,   311,
     314,   278,   280,   282,   338,   338,   340,   340,   319,   325,
     325,   338,    47,    48,   100,   117,   137,   148,   187,   226,
     229,   335,   336,   337,   335,   335,   187,    85,   100,   137,
     173,   187,   253,   286,   291,    84,   100,   120,   137,   187,
     263,   286,   291,   101,   150,   151,   152,   187,   289,   154,
     155,   156,   157,   187,   294,   335,   332,   332,   335,   335,
      86,    87,    88,    89,   191,   277,   338,   338,    78,    79,
      80,    81,    83,   174,   176,   298,   338,   188,   338,    82,
      98,   187,   310,    98,   173,   175,   177,   187,   312,    98,
     173,   177,   187,   315,   128,   129,   130,   131,   189,   279,
     134,   135,   136,   189,   281,   132,   133,   189,   283,   338,
     189,   340,   189,   100,   160,   161,   187,   320,   100,   101,
     127,   137,   178,   187,   326,   187,   189,   186,   186,   336,
     340,   331,   340,   336,   340,   336,   331,   254,   336,   336,
     188,   331,   110,   111,   290,   188,   188,   188,   188,   338,
     188,   188,   336,   340,   336,   340,   336,   340,   336,   340,
     340,   338,   338,   338,    78,    79,   174,   176,   313,   338,
     188,   338,   338,    78,    79,   174,   176,   316,   188,   338,
     338,   340,   336,   335,   335,   336,   108,   109,   328,   188,
     331,   188,   230,   227,   179,   180,   255,   264,   338,   338,
     338,   338,   338,   338,   340,   340,   340,   189,   189,   327,
     338,   231,    57,    58,   187,   228,   265,   338,   338,   338,
     338,   338,   338,   189,   340,   340,   282,   338,    48,    84,
     100,   137,   187,   232,   235,   335,   335,   134,   135,   136,
     189,   266,   338,   338,   338,   338,   189,   338,   189,   189,
     189,   189,   186,   186,   336,   331,   189,   189,   189,   189,
     189,   233,   236,    49,    50,    51,    52,    53,    55,    56,
      59,    60,    61,    62,    63,    64,    65,    66,   187,   234,
     237,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,    48,   100,   137,   138,
     187,   238,   186,   336,   331,   188,   240,   239,    53,    54,
     187,   241,   284,   335,   335,   139,   140,   141,   189,   285
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:

    {
        eq::server::Global::instance()->setConfigFAttribute( 
            eq::server::Config::FATTR_VERSION, (yyvsp[(2) - (3)]._float) );
    ;}
    break;

  case 10:

    {
         eq::server::Global::instance()->setConnectionSAttribute(
             eq::server::ConnectionDescription::SATTR_HOSTNAME, (yyvsp[(2) - (2)]._string) );
     ;}
    break;

  case 11:

    { 
         eq::server::Global::instance()->setConnectionIAttribute( 
             eq::server::ConnectionDescription::IATTR_TYPE, (yyvsp[(2) - (2)]._connectionType) ); 
     ;}
    break;

  case 12:

    {
         eq::server::Global::instance()->setConnectionIAttribute(
             eq::server::ConnectionDescription::IATTR_PORT, (yyvsp[(2) - (2)]._unsigned) );
     ;}
    break;

  case 13:

    {
         eq::server::Global::instance()->setConnectionSAttribute(
             eq::server::ConnectionDescription::SATTR_FILENAME, (yyvsp[(2) - (2)]._string) );
     ;}
    break;

  case 14:

    {
         eq::server::Global::instance()->setConnectionIAttribute(
             eq::server::ConnectionDescription::IATTR_BANDWIDTH, (yyvsp[(2) - (2)]._unsigned) );
     ;}
    break;

  case 15:

    {
         eq::server::Global::instance()->setConfigFAttribute(
             eq::server::Config::FATTR_EYE_BASE, (yyvsp[(2) - (2)]._float) );
     ;}
    break;

  case 16:

    {
         eq::server::Global::instance()->setConfigIAttribute(
             eq::server::Config::IATTR_ROBUSTNESS, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 17:

    {
         eq::server::Global::instance()->setNodeSAttribute(
             eq::server::Node::SATTR_LAUNCH_COMMAND, (yyvsp[(2) - (2)]._string) );
     ;}
    break;

  case 18:

    {
         eq::server::Global::instance()->setNodeCAttribute(
             eq::server::Node::CATTR_LAUNCH_COMMAND_QUOTE, (yyvsp[(2) - (2)]._character) );
     ;}
    break;

  case 19:

    {
         eq::server::Global::instance()->setNodeIAttribute(
             eq::server::Node::IATTR_THREAD_MODEL, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 20:

    {
         eq::server::Global::instance()->setNodeIAttribute(
             eq::server::Node::IATTR_LAUNCH_TIMEOUT, (yyvsp[(2) - (2)]._unsigned) );
     ;}
    break;

  case 21:

    {
         EQWARN << "Ignoring deprecated attribute Node::IATTR_HINT_STATISTICS"
                << std::endl;
     ;}
    break;

  case 22:

    {
         eq::server::Global::instance()->setPipeIAttribute(
             eq::server::Pipe::IATTR_HINT_THREAD, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 23:

    {
         eq::server::Global::instance()->setPipeIAttribute(
             eq::server::Pipe::IATTR_HINT_CUDA_GL_INTEROP, (yyvsp[(2) - (2)]._int) );
	 ;}
    break;

  case 24:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_HINT_STEREO, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 25:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_HINT_DOUBLEBUFFER, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 26:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_HINT_FULLSCREEN, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 27:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_HINT_DECORATION, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 28:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_HINT_SWAPSYNC, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 29:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_HINT_DRAWABLE, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 30:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_HINT_STATISTICS, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 31:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_HINT_SCREENSAVER, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 32:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_PLANES_COLOR, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 33:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_PLANES_ALPHA, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 34:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_PLANES_DEPTH, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 35:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_PLANES_STENCIL, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 36:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_PLANES_ACCUM, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 37:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_PLANES_ACCUM_ALPHA, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 38:

    {
         eq::server::Global::instance()->setWindowIAttribute(
             eq::server::Window::IATTR_PLANES_SAMPLES, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 39:

    {
         eq::server::Global::instance()->setChannelIAttribute(
             eq::server::Channel::IATTR_HINT_STATISTICS, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 40:

    {
         eq::server::Global::instance()->setChannelIAttribute(
             eq::server::Channel::IATTR_HINT_SENDTOKEN, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 41:

    { 
         eq::server::Global::instance()->setCompoundIAttribute( 
             eq::server::Compound::IATTR_STEREO_MODE, (yyvsp[(2) - (2)]._int) ); 
     ;}
    break;

  case 42:

    { 
         eq::server::Global::instance()->setCompoundIAttribute( 
             eq::server::Compound::IATTR_STEREO_ANAGLYPH_LEFT_MASK, (yyvsp[(2) - (2)]._unsigned) ); 
     ;}
    break;

  case 43:

    { 
         eq::server::Global::instance()->setCompoundIAttribute( 
             eq::server::Compound::IATTR_STEREO_ANAGLYPH_RIGHT_MASK, (yyvsp[(2) - (2)]._unsigned) ); 
     ;}
    break;

  case 44:

    {
         EQWARN << "ignoring removed attribute EQ_COMPOUND_IATTR_UPDATE_FOV"
                << std::endl;
     ;}
    break;

  case 45:

    {
         eq::server::Global::instance()->setCompoundIAttribute(
             eq::server::Compound::IATTR_HINT_OFFSET, (yyvsp[(2) - (2)]._int) );
     ;}
    break;

  case 46:

    { (yyval._connectionType) = co::CONNECTIONTYPE_TCPIP; ;}
    break;

  case 47:

    { (yyval._connectionType) = co::CONNECTIONTYPE_SDP; ;}
    break;

  case 48:

    { (yyval._connectionType) = co::CONNECTIONTYPE_IB; ;}
    break;

  case 49:

    { (yyval._connectionType) = co::CONNECTIONTYPE_NAMEDPIPE; ;}
    break;

  case 50:

    { (yyval._connectionType) = co::CONNECTIONTYPE_MCIP; ;}
    break;

  case 51:

    { (yyval._connectionType) = co::CONNECTIONTYPE_PGM; ;}
    break;

  case 52:

    { (yyval._connectionType) = co::CONNECTIONTYPE_RSP; ;}
    break;

  case 53:

    { server = new eq::server::Server(); ;}
    break;

  case 57:

    { 
                connectionDescription = new eq::server::ConnectionDescription;
                connectionDescription->setHostname( "" );
                connectionDescription->port = EQ_DEFAULT_PORT;
            ;}
    break;

  case 58:

    { 
                server->addConnectionDescription( connectionDescription.get( ));
                connectionDescription = 0;
            ;}
    break;

  case 61:

    {
                config = new eq::server::Config( server );
                config->setName( filename );
                node = new eq::server::Node( config );
                node->setApplicationNode( true );
            ;}
    break;

  case 62:

    { config = 0; ;}
    break;

  case 66:

    { config->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 71:

    { config->setLatency( (yyvsp[(2) - (2)]._unsigned) ); ;}
    break;

  case 75:

    { config->setFAttribute( 
                             eq::server::Config::FATTR_EYE_BASE, (yyvsp[(2) - (2)]._float) ); ;}
    break;

  case 76:

    { config->setIAttribute( 
                                 eq::server::Config::IATTR_ROBUSTNESS, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 79:

    {
                                 node = new eq::server::Node( config );
                             ;}
    break;

  case 80:

    { node = 0; ;}
    break;

  case 81:

    {
                node = config->findApplicationNode();
                EQASSERT( node )
            ;}
    break;

  case 82:

    { node = 0; ;}
    break;

  case 85:

    { node->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 89:

    { connectionDescription = new eq::server::ConnectionDescription; ;}
    break;

  case 90:

    { 
                 node->addConnectionDescription( connectionDescription );
                 connectionDescription = 0;
             ;}
    break;

  case 93:

    { connectionDescription->type = (yyvsp[(2) - (2)]._connectionType); ;}
    break;

  case 94:

    { connectionDescription->setHostname((yyvsp[(2) - (2)]._string)); ;}
    break;

  case 95:

    { connectionDescription->setInterface((yyvsp[(2) - (2)]._string)); ;}
    break;

  case 96:

    { connectionDescription->port = (yyvsp[(2) - (2)]._unsigned); ;}
    break;

  case 97:

    { connectionDescription->bandwidth = (yyvsp[(2) - (2)]._unsigned); ;}
    break;

  case 98:

    { connectionDescription->setFilename((yyvsp[(2) - (2)]._string)); ;}
    break;

  case 101:

    { node->setSAttribute( eq::server::Node::SATTR_LAUNCH_COMMAND, (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 102:

    { node->setCAttribute( eq::server::Node::CATTR_LAUNCH_COMMAND_QUOTE,
                               (yyvsp[(2) - (2)]._character) ); ;}
    break;

  case 103:

    { node->setIAttribute( eq::server::Node::IATTR_THREAD_MODEL, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 104:

    { node->setIAttribute( eq::server::Node::IATTR_LAUNCH_TIMEOUT, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 105:

    {
            EQWARN
                << "Ignoring deprecated attribute Node::IATTR_HINT_STATISTICS"
                << std::endl;
        ;}
    break;

  case 106:

    {
                eqPipe = new eq::server::Pipe( node );
            ;}
    break;

  case 107:

    { eqPipe = 0; ;}
    break;

  case 112:

    { eqPipe->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 113:

    { eqPipe->setPort( (yyvsp[(2) - (2)]._unsigned) ); ;}
    break;

  case 114:

    { eqPipe->setDevice( (yyvsp[(2) - (2)]._unsigned) ); ;}
    break;

  case 115:

    {
            eqPipe->setPixelViewport( eq::PixelViewport( (int)(yyvsp[(2) - (2)]._viewport)[0], (int)(yyvsp[(2) - (2)]._viewport)[1],
                                                      (int)(yyvsp[(2) - (2)]._viewport)[2], (int)(yyvsp[(2) - (2)]._viewport)[3] ));
        ;}
    break;

  case 118:

    { eqPipe->setIAttribute( eq::server::Pipe::IATTR_HINT_THREAD, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 119:

    { eqPipe->setIAttribute( eq::server::Pipe::IATTR_HINT_CUDA_GL_INTEROP, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 120:

    {
                window = new eq::server::Window( eqPipe );
                window->init(); // not in ctor, virtual method
            ;}
    break;

  case 121:

    { window = 0; ;}
    break;

  case 126:

    { window->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 127:

    {
            if( (yyvsp[(2) - (2)]._viewport)[2] > 1 || (yyvsp[(2) - (2)]._viewport)[3] > 1 )
                window->setPixelViewport( eq::PixelViewport( (int)(yyvsp[(2) - (2)]._viewport)[0], 
                                          (int)(yyvsp[(2) - (2)]._viewport)[1], (int)(yyvsp[(2) - (2)]._viewport)[2], (int)(yyvsp[(2) - (2)]._viewport)[3] ));
            else
                window->setViewport( eq::Viewport((yyvsp[(2) - (2)]._viewport)[0], (yyvsp[(2) - (2)]._viewport)[1], (yyvsp[(2) - (2)]._viewport)[2], (yyvsp[(2) - (2)]._viewport)[3])); 
        ;}
    break;

  case 130:

    { window->setIAttribute( eq::server::Window::IATTR_HINT_STEREO, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 131:

    { window->setIAttribute( eq::server::Window::IATTR_HINT_DOUBLEBUFFER, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 132:

    { window->setIAttribute( eq::server::Window::IATTR_HINT_FULLSCREEN, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 133:

    { window->setIAttribute( eq::server::Window::IATTR_HINT_DECORATION, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 134:

    { window->setIAttribute( eq::server::Window::IATTR_HINT_SWAPSYNC, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 135:

    { window->setIAttribute( eq::server::Window::IATTR_HINT_DRAWABLE, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 136:

    { window->setIAttribute( eq::server::Window::IATTR_HINT_STATISTICS, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 137:

    { window->setIAttribute( eq::server::Window::IATTR_HINT_SCREENSAVER, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 138:

    { window->setIAttribute( eq::server::Window::IATTR_PLANES_COLOR, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 139:

    { window->setIAttribute( eq::server::Window::IATTR_PLANES_ALPHA, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 140:

    { window->setIAttribute( eq::server::Window::IATTR_PLANES_DEPTH, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 141:

    { window->setIAttribute( eq::server::Window::IATTR_PLANES_STENCIL, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 142:

    { window->setIAttribute( eq::server::Window::IATTR_PLANES_ACCUM, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 143:

    { window->setIAttribute( eq::server::Window::IATTR_PLANES_ACCUM_ALPHA, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 144:

    { window->setIAttribute( eq::server::Window::IATTR_PLANES_SAMPLES, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 145:

    {
                channel = new eq::server::Channel( window );
                channel->init(); // not in ctor, virtual method
            ;}
    break;

  case 146:

    { channel = 0; ;}
    break;

  case 149:

    { channel->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 151:

    {
            if( (yyvsp[(2) - (2)]._viewport)[2] > 1 || (yyvsp[(2) - (2)]._viewport)[3] > 1 )
                channel->setPixelViewport( eq::PixelViewport( (int)(yyvsp[(2) - (2)]._viewport)[0],
                                          (int)(yyvsp[(2) - (2)]._viewport)[1], (int)(yyvsp[(2) - (2)]._viewport)[2], (int)(yyvsp[(2) - (2)]._viewport)[3] ));
            else
                channel->setViewport(eq::Viewport( (yyvsp[(2) - (2)]._viewport)[0], (yyvsp[(2) - (2)]._viewport)[1], (yyvsp[(2) - (2)]._viewport)[2], (yyvsp[(2) - (2)]._viewport)[3]));
        ;}
    break;

  case 152:

    { flags = eq::server::Channel::FB_WINDOW; ;}
    break;

  case 153:

    { channel->setDrawable( flags ); flags = 0; ;}
    break;

  case 156:

    { channel->setIAttribute( eq::server::Channel::IATTR_HINT_STATISTICS, 
                                  (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 157:

    { channel->setIAttribute( eq::server::Channel::IATTR_HINT_SENDTOKEN,
                                  (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 158:

    { observer = new eq::server::Observer( config );;}
    break;

  case 159:

    { observer = 0; ;}
    break;

  case 162:

    { observer->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 163:

    { observer->setEyeBase( (yyvsp[(2) - (2)]._float) ); ;}
    break;

  case 164:

    { layout = new eq::server::Layout( config ); ;}
    break;

  case 165:

    { layout = 0; ;}
    break;

  case 168:

    { layout->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 170:

    { view = new eq::server::View( layout ); ;}
    break;

  case 171:

    { view = 0; ;}
    break;

  case 174:

    { view->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 175:

    { view->changeMode( eq::server::View::MODE_MONO ); ;}
    break;

  case 177:

    { view->setViewport( eq::Viewport( (yyvsp[(2) - (2)]._viewport)[0], (yyvsp[(2) - (2)]._viewport)[1], (yyvsp[(2) - (2)]._viewport)[2], (yyvsp[(2) - (2)]._viewport)[3] ));;}
    break;

  case 178:

    { view->setWall( wall ); ;}
    break;

  case 179:

    { view->setProjection( projection ); ;}
    break;

  case 180:

    {
          eq::server::Observer* observer = 
              config->find< eq::server::Observer >( (yyvsp[(2) - (2)]._string) );
          if( !observer )
          {
              yyerror( "No observer of the given name" );
              YYERROR;
          }
          else
              view->setObserver( observer ); 
      ;}
    break;

  case 181:

    {
          const eq::server::ObserverPath path( (yyvsp[(2) - (2)]._unsigned) );
          eq::server::Observer* observer = config->getObserver( path );
          if( !observer )
          {
              yyerror( "No observer of the given index" );
              YYERROR;
          }
          else
              view->setObserver( observer ); 
      ;}
    break;

  case 182:

    { view->changeMode( eq::server::View::MODE_MONO ); ;}
    break;

  case 183:

    { view->changeMode( eq::server::View::MODE_STEREO ); ;}
    break;

  case 184:

    { canvas = new eq::server::Canvas( config ); ;}
    break;

  case 185:

    { config->activateCanvas( canvas ); canvas = 0; ;}
    break;

  case 188:

    { canvas->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 189:

    {
          eq::server::Layout* layout = config->find< eq::server::Layout >( (yyvsp[(2) - (2)]._string) );
          if( !layout )
          {
              yyerror( "No layout of the given name" );
              YYERROR;
          }
          else
              canvas->addLayout( layout ); 
      ;}
    break;

  case 190:

    {
          const eq::server::LayoutPath path( (yyvsp[(2) - (2)]._unsigned) );
          eq::server::Layout* layout = config->getLayout( path );
          if( !layout )
          {
              yyerror( "No layout of the given index" );
              YYERROR;
          }
          else
              canvas->addLayout( layout ); 
      ;}
    break;

  case 191:

    {
          canvas->addLayout( 0 ); 
      ;}
    break;

  case 192:

    { canvas->setWall( wall ); ;}
    break;

  case 193:

    { canvas->setProjection( projection ); ;}
    break;

  case 195:

    { segment = new eq::server::Segment( canvas ); ;}
    break;

  case 199:

    { segment->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 200:

    {
            eq::server::Channel* channel = 
                config->find< eq::server::Channel >( (yyvsp[(2) - (2)]._string) );
            if( !channel )
            {
                yyerror( "No channel of the given name" );
                YYERROR;
            }
            else
                segment->setChannel( channel );
        ;}
    break;

  case 201:

    { segment->setEyes( eq::fabric::EYE_UNDEFINED );;}
    break;

  case 203:

    { segment->setViewport( eq::Viewport( (yyvsp[(2) - (2)]._viewport)[0], (yyvsp[(2) - (2)]._viewport)[1], (yyvsp[(2) - (2)]._viewport)[2], (yyvsp[(2) - (2)]._viewport)[3] ));;}
    break;

  case 204:

    { segment->setWall( wall ); ;}
    break;

  case 205:

    { segment->setProjection( projection ); ;}
    break;

  case 208:

    { segment->enableEye( eq::fabric::EYE_CYCLOP ); ;}
    break;

  case 209:

    { segment->enableEye( eq::fabric::EYE_LEFT ); ;}
    break;

  case 210:

    { segment->enableEye( eq::fabric::EYE_RIGHT ); ;}
    break;

  case 211:

    {
                  if( eqCompound )
                      eqCompound = new eq::server::Compound( eqCompound );
                  else
                      eqCompound = new eq::server::Compound( config );
              ;}
    break;

  case 212:

    { eqCompound = eqCompound->getParent(); ;}
    break;

  case 216:

    { eqCompound->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 217:

    {
          eq::server::Channel* channel = 
              config->find< eq::server::Channel >( (yyvsp[(2) - (2)]._string) );
          if( !channel )
          {
              yyerror( "No channel of the given name" );
              YYERROR;
          }
          else
              eqCompound->setChannel( channel );
      ;}
    break;

  case 218:

    {
          if( !segment || !view )
          {
              yyerror( "Incomplete channel reference (view or segment missing)" );
              YYERROR;
          }
          else
          {
              eq::server::Channel* channel = 
                  config->findChannel( segment, view );
              if( channel )
                  eqCompound->setChannel( channel );
              else
              {
                  yyerror( "No channel for the given view and segment" );
                  YYERROR;
              }
          }

          canvas = 0;
          segment = 0;
          layout = 0;
          view = 0;
      ;}
    break;

  case 219:

    { eqCompound->setTasks( eq::fabric::TASK_NONE ); ;}
    break;

  case 221:

    { eqCompound->setEyes( eq::fabric::EYE_UNDEFINED );;}
    break;

  case 223:

    { flags = eq::Frame::BUFFER_NONE; ;}
    break;

  case 224:

    { eqCompound->setBuffers( flags ); flags = 0; ;}
    break;

  case 225:

    { eqCompound->setViewport( eq::Viewport( (yyvsp[(2) - (2)]._viewport)[0], (yyvsp[(2) - (2)]._viewport)[1], (yyvsp[(2) - (2)]._viewport)[2], (yyvsp[(2) - (2)]._viewport)[3] ));;}
    break;

  case 226:

    { eqCompound->setRange( eq::Range( (yyvsp[(3) - (5)]._float), (yyvsp[(4) - (5)]._float) )); ;}
    break;

  case 227:

    { eqCompound->setPeriod( (yyvsp[(2) - (2)]._unsigned) ); ;}
    break;

  case 228:

    { eqCompound->setPhase( (yyvsp[(2) - (2)]._unsigned) ); ;}
    break;

  case 229:

    { eqCompound->setZoom( eq::Zoom( (yyvsp[(3) - (5)]._float), (yyvsp[(4) - (5)]._float) )); ;}
    break;

  case 230:

    { eqCompound->setPixel( eq::Pixel( (yyvsp[(3) - (7)]._unsigned), (yyvsp[(4) - (7)]._unsigned), (yyvsp[(5) - (7)]._unsigned), (yyvsp[(6) - (7)]._unsigned) )); ;}
    break;

  case 231:

    { eqCompound->setSubPixel( eq::SubPixel( (yyvsp[(3) - (5)]._unsigned), (yyvsp[(4) - (5)]._unsigned) )); ;}
    break;

  case 232:

    { eqCompound->setWall( wall ); ;}
    break;

  case 233:

    { eqCompound->setProjection( projection ); ;}
    break;

  case 240:

    {
            canvas = 0;
            segment = config->getSegment( eq::server::SegmentPath( 0 ));
            layout = 0;
            view = config->getView( eq::server::ViewPath( 0 ));;
        ;}
    break;

  case 244:

    {
            canvas = config->find< eq::server::Canvas >( (yyvsp[(2) - (2)]._string) );
            if( !canvas )
                yyerror( "Can't find canvas" );
            segment = canvas->getSegment( eq::server::SegmentPath( 0 ));
        ;}
    break;

  case 245:

    {
            canvas = config->getCanvas( eq::server::CanvasPath( (yyvsp[(2) - (2)]._unsigned) ));
            if( !canvas )
                yyerror( "Can't find canvas" );
            segment = canvas->getSegment( eq::server::SegmentPath( 0 ));
        ;}
    break;

  case 246:

    { 
            if( canvas )
                segment = canvas->findSegment( (yyvsp[(2) - (2)]._string) ); 
            else
                segment = config->find< eq::server::Segment >( (yyvsp[(2) - (2)]._string) );
        ;}
    break;

  case 247:

    {
            if( canvas )
                segment = canvas->getSegment( eq::server::SegmentPath( (yyvsp[(2) - (2)]._unsigned) ));
            else
                segment = config->getSegment( eq::server::SegmentPath( (yyvsp[(2) - (2)]._unsigned) ));
        ;}
    break;

  case 248:

    {
            layout = config->find< eq::server::Layout >( (yyvsp[(2) - (2)]._string) );
            if( !layout )
                yyerror( "Can't find layout" );
            view = layout->getView( eq::server::ViewPath( 0 ));;
        ;}
    break;

  case 249:

    {
            layout = config->getLayout( eq::server::LayoutPath( (yyvsp[(2) - (2)]._unsigned) ));
            if( !layout )
                yyerror( "Can't find layout" );
            view = layout->getView( eq::server::ViewPath( 0 ));;
        ;}
    break;

  case 250:

    { 
            if( layout )
                view = layout->findView( (yyvsp[(2) - (2)]._string) ); 
            else
                view = config->find< eq::server::View >( (yyvsp[(2) - (2)]._string) );
        ;}
    break;

  case 251:

    {
            if( layout )
                view = layout->getView( eq::server::ViewPath( (yyvsp[(2) - (2)]._unsigned) ));
            else
                view = config->getView( eq::server::ViewPath( (yyvsp[(2) - (2)]._unsigned) ));
        ;}
    break;

  case 254:

    { eqCompound->enableTask( eq::fabric::TASK_CLEAR ); ;}
    break;

  case 255:

    { eqCompound->enableTask( eq::fabric::TASK_DRAW ); ;}
    break;

  case 256:

    { eqCompound->enableTask( eq::fabric::TASK_ASSEMBLE );;}
    break;

  case 257:

    { eqCompound->enableTask( eq::fabric::TASK_READBACK );;}
    break;

  case 260:

    { eqCompound->enableEye( eq::fabric::EYE_CYCLOP ); ;}
    break;

  case 261:

    { eqCompound->enableEye( eq::fabric::EYE_LEFT ); ;}
    break;

  case 262:

    { eqCompound->enableEye( eq::fabric::EYE_RIGHT ); ;}
    break;

  case 265:

    { flags |= eq::Frame::BUFFER_COLOR; ;}
    break;

  case 266:

    { flags |= eq::Frame::BUFFER_DEPTH; ;}
    break;

  case 269:

    { flags |= eq::server::Channel::FBO_COLOR; ;}
    break;

  case 270:

    { flags |= eq::server::Channel::FBO_DEPTH; ;}
    break;

  case 271:

    { flags |= eq::server::Channel::FBO_STENCIL; ;}
    break;

  case 272:

    { wall = eq::Wall(); ;}
    break;

  case 276:

    { wall.bottomLeft = eq::Vector3f( (yyvsp[(3) - (6)]._float), (yyvsp[(4) - (6)]._float), (yyvsp[(5) - (6)]._float) ); ;}
    break;

  case 277:

    { wall.bottomRight = eq::Vector3f( (yyvsp[(3) - (6)]._float), (yyvsp[(4) - (6)]._float), (yyvsp[(5) - (6)]._float) ); ;}
    break;

  case 278:

    { wall.topLeft = eq::Vector3f( (yyvsp[(3) - (6)]._float), (yyvsp[(4) - (6)]._float), (yyvsp[(5) - (6)]._float) ); ;}
    break;

  case 280:

    { wall.type = eq::Wall::TYPE_FIXED; ;}
    break;

  case 281:

    { wall.type = eq::Wall::TYPE_HMD; ;}
    break;

  case 282:

    { projection = eq::Projection(); ;}
    break;

  case 286:

    { projection.origin = eq::Vector3f( (yyvsp[(3) - (6)]._float), (yyvsp[(4) - (6)]._float), (yyvsp[(5) - (6)]._float) ); ;}
    break;

  case 287:

    { projection.distance = (yyvsp[(2) - (2)]._float); ;}
    break;

  case 288:

    { projection.fov = eq::Vector2f( (yyvsp[(3) - (5)]._float), (yyvsp[(4) - (5)]._float) ); ;}
    break;

  case 289:

    { projection.hpr = eq::Vector3f( (yyvsp[(3) - (6)]._float), (yyvsp[(4) - (6)]._float), (yyvsp[(5) - (6)]._float) ); ;}
    break;

  case 290:

    {
        EQWARN << "Deprecated loadBalancer specification, "
               << " use new ???_equalizer grammar" << std::endl;

        dfrEqualizer = 0;
        loadEqualizer = 0;
    ;}
    break;

  case 294:

    {
        if( loadEqualizer )
            loadEqualizer->setDamping( (yyvsp[(2) - (2)]._float) );
        else if( dfrEqualizer )
            dfrEqualizer->setDamping( (yyvsp[(2) - (2)]._float) );
    ;}
    break;

  case 295:

    { loadEqualizer->setAssembleOnlyLimit( (yyvsp[(2) - (2)]._float) ) ;}
    break;

  case 296:

    { dfrEqualizer->setFrameRate( (yyvsp[(2) - (2)]._float) ) ;}
    break;

  case 297:

    { loadEqualizer->setBoundary( eq::Vector2i( (yyvsp[(3) - (5)]._unsigned), (yyvsp[(4) - (5)]._unsigned) )) ;}
    break;

  case 298:

    { loadEqualizer->setBoundary( (yyvsp[(2) - (2)]._float) ) ;}
    break;

  case 299:

    {
        loadEqualizer = new eq::server::LoadEqualizer;
        loadEqualizer->setMode( eq::server::LoadEqualizer::MODE_2D );
        eqCompound->addEqualizer( loadEqualizer );
    ;}
    break;

  case 300:

    {
        loadEqualizer = new eq::server::LoadEqualizer;
        loadEqualizer->setMode( eq::server::LoadEqualizer::MODE_DB );
        eqCompound->addEqualizer( loadEqualizer );
    ;}
    break;

  case 301:

    {
        loadEqualizer = new eq::server::LoadEqualizer;
        loadEqualizer->setMode( eq::server::LoadEqualizer::MODE_HORIZONTAL );
        eqCompound->addEqualizer( loadEqualizer );
    ;}
    break;

  case 302:

    {
        loadEqualizer = new eq::server::LoadEqualizer;
        loadEqualizer->setMode( eq::server::LoadEqualizer::MODE_VERTICAL );
        eqCompound->addEqualizer( loadEqualizer );
    ;}
    break;

  case 303:

    {
        eqCompound->addEqualizer( new eq::server::FramerateEqualizer );
    ;}
    break;

  case 304:

    {
        dfrEqualizer = new eq::server::DFREqualizer;
        eqCompound->addEqualizer( dfrEqualizer );
    ;}
    break;

  case 305:

    {
        eqCompound->addEqualizer( new eq::server::MonitorEqualizer );
    ;}
    break;

  case 312:

    { dfrEqualizer = new eq::server::DFREqualizer; ;}
    break;

  case 313:

    {
        eqCompound->addEqualizer( dfrEqualizer );
        dfrEqualizer = 0; 
    ;}
    break;

  case 314:

    {
        eqCompound->addEqualizer( new eq::server::FramerateEqualizer );
    ;}
    break;

  case 315:

    { loadEqualizer = new eq::server::LoadEqualizer; ;}
    break;

  case 316:

    {
        eqCompound->addEqualizer( loadEqualizer );
        loadEqualizer = 0; 
    ;}
    break;

  case 317:

    { treeEqualizer = new eq::server::TreeEqualizer; ;}
    break;

  case 318:

    {
        eqCompound->addEqualizer( treeEqualizer );
        treeEqualizer = 0; 
    ;}
    break;

  case 319:

    {
        eqCompound->addEqualizer( new eq::server::MonitorEqualizer );
    ;}
    break;

  case 320:

    {
        eqCompound->addEqualizer( new eq::server::ViewEqualizer );
    ;}
    break;

  case 323:

    { dfrEqualizer->setDamping( (yyvsp[(2) - (2)]._float) ); ;}
    break;

  case 324:

    { dfrEqualizer->setFrameRate( (yyvsp[(2) - (2)]._float) ) ;}
    break;

  case 327:

    { loadEqualizer->setDamping( (yyvsp[(2) - (2)]._float) ); ;}
    break;

  case 328:

    { loadEqualizer->setBoundary( eq::Vector2i( (yyvsp[(3) - (5)]._unsigned), (yyvsp[(4) - (5)]._unsigned) )) ;}
    break;

  case 329:

    { loadEqualizer->setAssembleOnlyLimit( (yyvsp[(2) - (2)]._float) ) ;}
    break;

  case 330:

    { loadEqualizer->setBoundary( (yyvsp[(2) - (2)]._float) ) ;}
    break;

  case 331:

    { loadEqualizer->setMode( (yyvsp[(2) - (2)]._loadEqualizerMode) ); ;}
    break;

  case 332:

    { (yyval._loadEqualizerMode) = eq::server::LoadEqualizer::MODE_2D; ;}
    break;

  case 333:

    { (yyval._loadEqualizerMode) = eq::server::LoadEqualizer::MODE_DB; ;}
    break;

  case 334:

    { (yyval._loadEqualizerMode) = eq::server::LoadEqualizer::MODE_HORIZONTAL; ;}
    break;

  case 335:

    { (yyval._loadEqualizerMode) = eq::server::LoadEqualizer::MODE_VERTICAL; ;}
    break;

  case 338:

    { treeEqualizer->setDamping( (yyvsp[(2) - (2)]._float) ); ;}
    break;

  case 339:

    { treeEqualizer->setBoundary( eq::Vector2i( (yyvsp[(3) - (5)]._unsigned), (yyvsp[(4) - (5)]._unsigned) )) ;}
    break;

  case 340:

    { treeEqualizer->setBoundary( (yyvsp[(2) - (2)]._float) ) ;}
    break;

  case 341:

    { treeEqualizer->setMode( (yyvsp[(2) - (2)]._treeEqualizerMode) ); ;}
    break;

  case 342:

    { (yyval._treeEqualizerMode) = eq::server::TreeEqualizer::MODE_2D; ;}
    break;

  case 343:

    { (yyval._treeEqualizerMode) = eq::server::TreeEqualizer::MODE_DB; ;}
    break;

  case 344:

    { (yyval._treeEqualizerMode) = eq::server::TreeEqualizer::MODE_HORIZONTAL; ;}
    break;

  case 345:

    { (yyval._treeEqualizerMode) = eq::server::TreeEqualizer::MODE_VERTICAL; ;}
    break;

  case 346:

    { swapBarrier = new eq::server::SwapBarrier; ;}
    break;

  case 347:

    { 
            eqCompound->setSwapBarrier( swapBarrier );
            swapBarrier = 0;
        ;}
    break;

  case 350:

    { swapBarrier->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 351:

    { swapBarrier->setNVSwapGroup( (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 352:

    { swapBarrier->setNVSwapBarrier( (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 353:

    { frame = new eq::server::Frame; ;}
    break;

  case 354:

    { 
            eqCompound->addOutputFrame( frame );
            frame = 0;
        ;}
    break;

  case 355:

    { frame = new eq::server::Frame; ;}
    break;

  case 356:

    { 
            eqCompound->addInputFrame( frame );
            frame = 0;
        ;}
    break;

  case 359:

    { frame->setName( (yyvsp[(2) - (2)]._string) ); ;}
    break;

  case 361:

    { frame->setViewport(eq::Viewport( (yyvsp[(2) - (2)]._viewport)[0], (yyvsp[(2) - (2)]._viewport)[1], (yyvsp[(2) - (2)]._viewport)[2], (yyvsp[(2) - (2)]._viewport)[3])); ;}
    break;

  case 362:

    { flags = eq::Frame::BUFFER_NONE; ;}
    break;

  case 363:

    { frame->setBuffers( flags ); flags = 0; ;}
    break;

  case 364:

    { frame->setZoom( eq::Zoom( (yyvsp[(3) - (5)]._float), (yyvsp[(4) - (5)]._float) )); ;}
    break;

  case 365:

    { frame->setType( eq::Frame::TYPE_TEXTURE ); ;}
    break;

  case 366:

    { frame->setType( eq::Frame::TYPE_MEMORY ); ;}
    break;

  case 369:

    { eqCompound->setIAttribute( eq::server::Compound::IATTR_STEREO_MODE, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 370:

    { eqCompound->setIAttribute( 
                eq::server::Compound::IATTR_STEREO_ANAGLYPH_LEFT_MASK, (yyvsp[(2) - (2)]._unsigned) ); ;}
    break;

  case 371:

    { eqCompound->setIAttribute( 
                eq::server::Compound::IATTR_STEREO_ANAGLYPH_RIGHT_MASK, (yyvsp[(2) - (2)]._unsigned) ); ;}
    break;

  case 372:

    { EQWARN << "ignoring removed attribute update_FOV" << std::endl; ;}
    break;

  case 373:

    { eqCompound->setIAttribute(eq::server::Compound::IATTR_HINT_OFFSET, (yyvsp[(2) - (2)]._int) ); ;}
    break;

  case 374:

    { 
         (yyval._viewport)[0] = (yyvsp[(2) - (6)]._float);
         (yyval._viewport)[1] = (yyvsp[(3) - (6)]._float);
         (yyval._viewport)[2] = (yyvsp[(4) - (6)]._float);
         (yyval._viewport)[3] = (yyvsp[(5) - (6)]._float);
     ;}
    break;

  case 375:

    { (yyval._unsigned) = (yyvsp[(2) - (3)]._unsigned); ;}
    break;

  case 376:

    { (yyval._unsigned) =eq::server::Compound::COLOR_MASK_NONE; ;}
    break;

  case 377:

    { (yyval._unsigned) = (yyvsp[(1) - (1)]._unsigned); ;}
    break;

  case 378:

    { (yyval._unsigned) = ((yyvsp[(1) - (2)]._unsigned) | (yyvsp[(2) - (2)]._unsigned));;}
    break;

  case 379:

    { (yyval._unsigned) = eq::server::Compound::COLOR_MASK_RED; ;}
    break;

  case 380:

    { (yyval._unsigned) = eq::server::Compound::COLOR_MASK_GREEN; ;}
    break;

  case 381:

    { (yyval._unsigned) = eq::server::Compound::COLOR_MASK_BLUE; ;}
    break;

  case 382:

    { (yyval._int) = eq::fabric::ON; ;}
    break;

  case 383:

    { (yyval._int) = eq::fabric::OFF; ;}
    break;

  case 384:

    { (yyval._int) = eq::fabric::AUTO; ;}
    break;

  case 385:

    { (yyval._int) = eq::fabric::FASTEST; ;}
    break;

  case 386:

    { (yyval._int) = eq::fabric::HORIZONTAL; ;}
    break;

  case 387:

    { (yyval._int) = eq::fabric::NICEST; ;}
    break;

  case 388:

    { (yyval._int) = eq::fabric::QUAD; ;}
    break;

  case 389:

    { (yyval._int) = eq::fabric::ANAGLYPH; ;}
    break;

  case 390:

    { (yyval._int) = eq::fabric::PASSIVE; ;}
    break;

  case 391:

    { (yyval._int) = eq::fabric::VERTICAL; ;}
    break;

  case 392:

    { (yyval._int) = eq::fabric::WINDOW; ;}
    break;

  case 393:

    { (yyval._int) = eq::fabric::FBO; ;}
    break;

  case 394:

    { (yyval._int) = eq::fabric::PBUFFER; ;}
    break;

  case 395:

    { (yyval._int) = eq::fabric::ASYNC; ;}
    break;

  case 396:

    { (yyval._int) = eq::fabric::DRAW_SYNC; ;}
    break;

  case 397:

    { (yyval._int) = eq::fabric::LOCAL_SYNC; ;}
    break;

  case 398:

    { (yyval._int) = eq::fabric::RGBA16F; ;}
    break;

  case 399:

    { (yyval._int) = eq::fabric::RGBA32F; ;}
    break;

  case 400:

    { (yyval._int) = (yyvsp[(1) - (1)]._int); ;}
    break;

  case 401:

    {
         static std::string stringBuf;
         stringBuf = yytext;
         stringBuf.erase( 0, 1 );                  // Leading '"'
         stringBuf.erase( stringBuf.size()-1, 1 ); // Trailing '"'
         (yyval._string) = stringBuf.c_str(); 
     ;}
    break;

  case 402:

    { (yyval._character) = yytext[1]; ;}
    break;

  case 403:

    { (yyval._float) = atof( yytext ); ;}
    break;

  case 404:

    { (yyval._float) = (yyvsp[(1) - (1)]._int); ;}
    break;

  case 405:

    { (yyval._int) = atoi( yytext ); ;}
    break;

  case 406:

    { (yyval._int) = (yyvsp[(1) - (1)]._unsigned); ;}
    break;

  case 407:

    { (yyval._unsigned) = atoi( yytext ); ;}
    break;



      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}





namespace eq
{
namespace server
{

//---------------------------------------------------------------------------
// loader
//---------------------------------------------------------------------------
ServerPtr Loader::loadFile( const std::string& filename )
{
    EQASSERTINFO( !eq::loader::loader, "Config file loader is not reentrant" );
    eq::loader::loader = this;

    yyin       = fopen( filename.c_str(), "r" );
    yyinString = 0;

    if( !yyin )
    {
        EQERROR << "Can't open config file " << filename << std::endl;
        eq::loader::loader = 0;
        return 0;
    }

    loader::filename = filename;
    loader::server = 0;
    config = 0;
    const bool error = ( eqLoader_parse() != 0 );

    fclose( yyin );
    eq::loader::loader = 0;
    loader::filename.clear();

    if( error )
        loader::server = 0;

    eq::server::ServerPtr server = loader::server;
    loader::server = 0;
    return server;
}

void Loader::_parseString( const char* data )
{
    EQASSERTINFO( !eq::loader::loader, "Config file loader is not reentrant" );
    eq::loader::loader = this;

    yyin       = 0;
    yyinString = data;

    loader::server = 0;
    config = 0;
    const bool error = ( eqLoader_parse() != 0 );
    if( error )
        loader::server = 0;

    eq::loader::loader = 0;
}

ServerPtr Loader::parseServer( const char* data )
{
    _parseString( data );

    eq::server::ServerPtr server = loader::server;
    loader::server = 0;
    return server;
}

}
}

