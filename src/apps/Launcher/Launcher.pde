/**
 * Shared Drawing Canvas (Client) 
 * by Alexander R. Galloway. 
 * 
 * The Processing Client class is instantiated by specifying a remote 
 * address and port number to which the socket connection should be made. 
 * Once the connection is made, the client may read (or write) data to the server.
 * Before running this program, start the Shared Drawing Canvas (Server) program.
 */


import processing.net.*;
import hypermedia.net.*;
import processing.opengl.*;
import javax.media.opengl.*;


Client cl;
UDP udp;
String input;
int data[];
int numstars=400;
final int SPREAD=64;
int CX,CY;
final float SPEED=0.9;

GL gl;

// Screen corner positions
final float a[] = { -.7075, 1.23, -.62 };
final float b[] = {  .7075, 1.23, -.62 };
final float c[] = { -.7075, 2.04, -.62 };

// Eye positions
final float l[] = { -.7075, 0.0, 0.0 };
final float r[] = {  .7075, 0.0, 0.0 };

Star[] s = new Star[numstars];

void setup() 
{
  size(1920, 1080, OPENGL);
  background(0);
  stroke(0);
  gl = ((PGraphicsOpenGL)g).gl;
  // Connect to the server's IP address and port
  cl = new Client(this, "127.0.0.1", 27000); // Replace with your server's IP and port
  // Write the handshake
  cl.write("data_on, 8000");
  
  udp = new UDP(this, 8000);
  
  udp.listen( true );
  
  CX=width/4 ; CY=height/2;
  for(int i=0;i<numstars;i++) {    
    s[i]= new Star();    
    s[i].SetPosition();  
  }
}

void draw_scene() 
{
  drawStarField();
  
  fill(255);
  //rect(560, 490, 200, 100);
  
  //rect(860, 490, 200, 100);
  
  rect(1160, 490, 200, 100);
  // Receive data from server
  
  
}

void draw() {
  gl.glViewport(0, 0, width, height);
  gl.glScissor(0, 0, width, height);
  
  background(255);
  
  draw_view(0,         0, width / 2, height, r); //right eye
  draw_view(width / 2, 0, width / 2, height, l); // left eye
  
}

void draw_view( int x, int y,
                int w, int h,
                float []eye) {

  gl.glViewport(x, y, w, h);
  gl.glScissor(x, y, w, h);
  
  fill(255);
  rect(0, 0, 100, 100);
  
  // Aply the projection
  //projection(eye, 1.0f, 36.0f);
  
  //gl.glLoadIdentity();
  //gl.glTranslatef(0.0f, 0.0f, 16.0f);
  //gl.glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
  draw_scene();  
  
  //fill(255);
  //rect(0, 0, 100, 100);
} 

float dot_product(float []a, float []b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void subtract(float []a, float []b, float []c)
{
    a[0] = b[0] - c[0];
    a[1] = b[1] - c[1];
    a[2] = b[2] - c[2];
}

void cross_product(float []a, float []b, float []c)
{
    a[0] = b[1] * c[2] - b[2] * c[1];
    a[1] = b[2] * c[0] - b[0] * c[2];
    a[2] = b[0] * c[1] - b[1] * c[0];
}

void normalize(float []a) {
  float d = (float) (1.0 / sqrt(dot_product(a, a)));
  
  a[0] *= d;
  a[1] *= d;
  a[2] *= d;
}

void projection(float []eye, float n, float f) {
  
  float va[], vb[], vc[];
  float vr[], vu[], vn[];
  
  va = new float[3];
  vb = new float[3];
  vc = new float[3];
  vr = new float[3];
  vu = new float[3];
  vn = new float[3];
            

  float l, r, b2, t, d, M[];
  M = new float[16];
    
  subtract(vr, b, a);
  subtract(vu, c, a);
  
  normalize(vr);
  normalize(vu);
  
  cross_product(vn, vr, vu);
  
  normalize(vn);
  
  // Compute the screen corner vectors
  subtract(va, a, eye);
  subtract(vb, b, eye);
  subtract(vc, c, eye);
  
  /* Compute the stortest distance from the eye to the screen. */

  d = -dot_product(va, vn);

  /* Compute the extents of the perpendicular projection. */

  l = dot_product(vr, va) * n / d;
  r = dot_product(vr, vb) * n / d;
  b2 = dot_product(vu, va) * n / d;
  t = dot_product(vu, vc) * n / d;

  /* Load the perpendicular projection on the projection matrix stack. */

  gl.glMatrixMode(gl.GL_PROJECTION);
  gl.glLoadIdentity();
  gl.glFrustum(l, r, b2, t, n, f);

  /* Rotate the projection by applying the screen orientation. */

  M[0] = vr[0]; M[4] = vr[1]; M[ 8] = vr[2]; M[12] = 0.0f;
  M[1] = vu[0]; M[5] = vu[1]; M[ 9] = vu[2]; M[13] = 0.0f;
  M[2] = vn[0]; M[6] = vn[1]; M[10] = vn[2]; M[14] = 0.0f;
  M[3] =  0.0f; M[7] =  0.0f; M[11] =  0.0f; M[15] = 1.0f;

  gl.glMultMatrixf(M, 0);

  /* Move the apex of the frustum to the origin. */

  gl.glTranslatef(-eye[0], -eye[1], -eye[2]);

  gl.glMatrixMode(gl.GL_MODELVIEW);
}

void receive( byte[] data, String ip, int port ) {	// <-- extended handler
  // get the "real" message =
  // forget the ";\n" at the end <-- !!! only for a communication with Pd !!!
  data = subset(data, 0, data.length-2);
  String message = new String( data );
  
  // print the result
  println( "receive: \""+message+"\" from "+ip+" on port "+port );
}

void drawStarField() {
  for(int i=0;i<numstars;i++) {    
    s[i].draw();  
  }
}


