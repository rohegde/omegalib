/*=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=
 *                                              Mirror Main 
 *=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=
 *  Title       :  HelloOmega : Main
 *  Authors     :  Dennis Chau ( Koracas@gmail.com )  
 *  Description :  
 *
 *=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=*/

#ifdef _WIN32
#  include <windows.h>
#endif

// includes, GL
#include "GL/glew.h"

#ifdef __linux__
#include <GL/glut.h>
#endif

#ifdef __APPLE__
#include <glut.h>
#endif

#ifdef _WIN32
#include "glut.h"
#endif


// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

//--------------------------------------------------------------------------------------------------//
//												#defines 
//--------------------------------------------------------------------------------------------------//
#define PI (3.141592653589793)


//--------------------------------------------------------------------------------------------------//
//							Constants
//--------------------------------------------------------------------------------------------------//
const int win_x = 0;
const int win_y = 0;

const unsigned int WIN_W = 1440;
const unsigned int WIN_H = 850;

const float FOV = 45.0;

const float COORD_X_MIN = 0.0;
const float COORD_X_MAX = 17.0;
const float COORD_Y_MIN = 0.0;
const float COORD_Y_MAX = 10.0;

const char *title = "Hello Omega";

const float BACK_GRD[ ] = { 0.0, 0.0, 0.0, 1.0 };

const float init_trans[ ] =  {0.0, 0.0, -0.75 };

//--------------------------------------------------------------------------------------------------//
//							Globals
//--------------------------------------------------------------------------------------------------//
// mouse controls
int g_mouse[2];
int g_mouse_old[2];
int g_mouse_click[2];
int g_mouse_buttons = 0;
time_t g_clicktime;

// view countrol 
int g_zoom_animation = 0.0f;
float g_translate[ ] = { init_trans[0], init_trans[1], init_trans[2] };

GLfloat angle = 0.0;

//--------------------------------------------------------------------------------------------------//
//							Prototypes
//--------------------------------------------------------------------------------------------------//
void initialize( );

// OPENGL functionality
void initGL( int argc, char** argv );

// Rendering Callbacks
void doCamFocus( );
void _beginWinCoords(void);
void _endWinCoords(void);
void _glPrint(int x, int y, const char *s, void *font, float r, float g, float b);
void cube (void);
void display( void );

void keyboard( unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

float coordPxToModel( float value);

//--------------------------------------------------------------------------------------------------//
//
//									Main
//
//--------------------------------------------------------------------------------------------------//
/************************************************************************************************
 * int main( int argc, char** argv)
 */
int main( int argc, char** argv)
{
	printf("[     %s     ]\n", title);
	
	initGL( argc, argv );
	
	// start rendering mainloop
    glutMainLoop();
	
}

/************************************************************************************************
 * initGL(int argc, char **argv)	
 *   Initialize GL
 */
void initGL( int argc, char** argv )
{
    // Create GL context
    glutInit( &argc, argv);
	
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	
	glutInitWindowPosition(win_x, win_y);
    
	glutInitWindowSize( WIN_W, WIN_H);
    
	//fullsreen mode
	glutGameModeString( "1440x900:32@60" );
		
	glutEnterGameMode(); //set glut to fullscreen using the settings in the line above		
		
	//lighting
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
	
	// register callbacks
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
	
    // initialize necessary OpenGL extensions
    glewInit();
	
    // default initialization
    glClearColor( BACK_GRD[0] , BACK_GRD[1] , BACK_GRD[2] , BACK_GRD[3] );
	
    // viewport
    glViewport( 0, 0, WIN_W, WIN_H);
	
    // projection
    glMatrixMode( GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(FOV, (GLfloat)WIN_W / (GLfloat)WIN_H, 0.1, 1000.0);
	
}//CUTBoolean initGL()

/************************************************************************************************
 *	void doCamFocus( float delta ) 
 */
void doCamFocus( ) 
{	
	float fov_radians;
	float z_Look = 0;
	
	fov_radians = FOV * PI/180.0;
	
	z_Look = (COORD_X_MAX/2.0) / tan( (fov_radians / 2.0) ); 
	
	gluLookAt(	COORD_X_MAX/2.0, COORD_Y_MAX/2.0, z_Look,
				COORD_X_MAX/2.0, COORD_Y_MAX/2.0, 0.0,
				0.0f,1.0f, 0.0f
			  );	
}// void doCamFocus( float delta ) 

/************************************************************************************************
 * beginWinCoords(void)
 */
void _beginWinCoords(void)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0, glutGet(GLUT_WINDOW_HEIGHT) - 1, 0.0);
	glScalef(1.0, -1.0, 1.0);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);
	
	glDisable (GL_DEPTH_TEST);
    glDisable (GL_LIGHTING);
    glDisable (GL_LIGHT0);
	
	glMatrixMode(GL_MODELVIEW);
}

/************************************************************************************************
 * endWinCoords(void)
 */
void _endWinCoords(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);	
	
}

/************************************************************************************************
 * void _glPrint(int x, int y, const char *s, void *font, float r, float g, float b)
 */
void _glPrint(int x, int y, const char *s, void *font, float r, float g, float b)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	int len = (int) strlen(s);
	for (int i = 0; i < len; i++) 
		glutBitmapCharacter(font, s[i]);
}

/************************************************************************************************
 * void cube (void) {
 */
void cube (void) {
	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 0.0);

		glTranslatef( COORD_X_MAX/2, COORD_Y_MAX/2, 0.0); 

		glRotatef(angle, 1.0, 0.0, 0.0); //rotate on the x axis
		glRotatef(angle, 0.0, 1.0, 0.0); //rotate on the y axis
		glRotatef(angle, 0.0, 0.0, 1.0); //rotate on the z axis
		
		glutSolidCube(2);
	}
	glPopMatrix();
}

/************************************************************************************************
 * void display()
 */
void display()
{		
	glClearColor( BACK_GRD[0] , BACK_GRD[1] , BACK_GRD[2] , BACK_GRD[3] );
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	doCamFocus();
	
    // set view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	//mouse controls
	if(g_zoom_animation > 0)
	{
		g_zoom_animation -= 0.1;
		g_translate[2] += 0.05;
		
		if(g_translate[2] > 5.0)
		{
			g_translate[2] = 5.0;
			g_zoom_animation = 0.0;
		}
	}
	glTranslatef(g_translate[0], g_translate[1], g_translate[2]);
	
	//instructions 
	_beginWinCoords();
	{
		char str_line[128]; 
		memset(str_line, 0, 128);	
		sprintf(str_line, "(esc) Exit");
		int len = (int) strlen(str_line);
		_glPrint( 0 , WIN_H, str_line, (void *) GLUT_BITMAP_9_BY_15, 1.0, 1.0, 1.0);
	}
	_endWinCoords();
	
	//cube
	cube();
	
    glutSwapBuffers();
    glutPostRedisplay();

	angle += .25;
}


/************************************************************************************************
 *	void keyboard(unsigned char key, int x, int y)
 */
void keyboard( unsigned char key, int /*x*/, int /*y*/)
{
	if (key==27) 
	{
		glutLeaveGameMode(); //set the resolution how it was
		exit( 0);
	}
}

/************************************************************************************************
 *	void mouse(int button, int state, int x, int y)
 */
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) 
	{
		g_mouse_buttons |= 1<<button;
	} 
	else if (state == GLUT_UP) 
	{
		g_mouse_buttons = 0;
		
		time_t end;
		time (&end);
		long int gap = end - g_clicktime;
		bool click=false;
		if(gap <= 1)
		{
			if((x == g_mouse_click[0]) && (y == g_mouse_click[1]))
			{
				g_mouse[0] = g_mouse_click[0];
				g_mouse[1] = g_mouse_click[1];
				g_mouse_click[0] = 	0;
				g_mouse_click[1] = 0;
				g_zoom_animation = 20.0;
				click=true;
			}
		}  
		
		if(click==false)
		{
			g_mouse_click[0] = x;
			g_mouse_click[1] = y;
		}
		time (&g_clicktime);
	}
	
	
	g_mouse_old[0] = x;
	g_mouse_old[1] = y;
	
	glutPostRedisplay();
	
}

/************************************************************************************************
 *	void motion(int x, int y)
 */
void motion(int x, int y)
{
	float dx, dy;
	dx = x - g_mouse_old[0];
	dy = y - g_mouse_old[1];
	float z = 0;
	
	if (g_mouse_buttons & 1) 
	{
		//rotate_x += dy * 0.2;
		//rotate_y += dx * 0.2;
		g_translate[0] += (dx * 0.01);
		g_translate[1] -= (dy * 0.01);
	} 
	else if (g_mouse_buttons & 4) 
	{
		z = g_translate[2] - (dy * 0.005);
		if((z >= -5.0) && (z <= 5.85))
			g_translate[2] = z;
	}
	
	
	g_mouse_old[0] = x;
	g_mouse_old[1] = y;
	
}

//--------------------------------------------------------------------------------------------------//
//
//									Gen Util Funcs 
//
//--------------------------------------------------------------------------------------------------//

/************************************************************************************************
 *	float coordPxToModel( float value)
 */
float coordPxToModel( float value)
{
	return value = ( value  - 0 ) / ( WIN_W - 0 ) * ( COORD_X_MAX - COORD_X_MIN) + COORD_X_MIN; 
}//void coordPxToModel( float &value)

