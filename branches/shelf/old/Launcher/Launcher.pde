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
import java.util.StringTokenizer;


Client cl;
UDP udp;
String input;
int data[];
int numstars=400;
final int SPREAD=64;
int CX,CY;
final float SPEED=0.6;

int udp_port = 8001;


Star[] s = new Star[numstars];
Pointer p = new Pointer();

Button b1 = new Button( 1920 / 2, 1080 / 5, 250, 100, "Calibration Tool" );
Button b2 = new Button( 1920 / 2, 2 * 1080 / 5, 250, 100, "MeshViewer" );
Button b3 = new Button( 1920 / 2, 3 * 1080 / 5, 250, 100, "CPig Project" );

void setup() 
{
  size(1920, 1080, OPENGL);
  background(0);
  stroke(0);
  // Connect to the server's IP address and port
  cl = new Client(this, "127.0.0.1", 27000); // Replace with your server's IP and port
  // Write the handshake
  cl.write("data_on, " + udp_port);
  
  udp = new UDP(this, udp_port);
  
  udp.listen( true );
  
  CX=width / 2 ; CY=height / 2;
  for(int i=0;i<numstars;i++) {    
    s[i]= new Star();    
    s[i].SetPosition();  
  }
}

void draw() {  
  background(0);
  
  drawStarField();
  p.draw();
  b1.draw();
  b2.draw();
  b3.draw();
}

void receive( byte[] data, String ip, int port ) {	// <-- extended handler
  // get the "real" message =
  // forget the ";\n" at the end <-- !!! only for a communication with Pd !!!
  data = subset(data, 0, data.length-2);
  String message = new String( data );
  
  // print the result
  println( "receive: \""+message+"\" from "+ip+" on port "+port ); 
  //Tokenize the result
  message = message.substring(4);
  println( "tokenized: \""+message+"\" from "+ip+" on port "+port ); 
  StringTokenizer st = new StringTokenizer(message, ",");
  
  
  int tokenNumber = 1;
  while( st.hasMoreTokens() ) {
    switch( tokenNumber ){
      case 1:
        float head_x = new Float(st.nextToken());
        float head_y = new Float(st.nextToken());
        float head_z = new Float(st.nextToken());
        
        //p.setx(head_x);
        //p.sety(head_y);
        //p.onScreen = true;
        break;
      case 6: // left hand
        //float 
        break;  
        
      case 12: // right hand
        float rhand_x = new Float(st.nextToken());
        float rhand_y = new Float(st.nextToken());
        float rhand_z = new Float(st.nextToken());
        p.onScreen = true;
        
        println("X: " + rhand_x + "Y: " + rhand_y + "Z: " + rhand_z);
        break;
        
      default:
        st.nextToken();
        st.nextToken();
        st.nextToken();
        break;
    }
    
    tokenNumber++;
  } 
}

void drawStarField() {
  for(int i=0;i<numstars;i++) {    
    s[i].draw();  
  }
}


