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


Client c;
UDP udp;
String input;
int data[];
int numstars=400;
final int SPREAD=64;
int CX,CY;
final float SPEED=0.9;

Star[] s = new Star[numstars];

void setup() 
{
  size(1920, 1080);
  background(0);
  stroke(0);
  // Connect to the server's IP address and port
  c = new Client(this, "127.0.0.1", 27000); // Replace with your server's IP and port
  // Write the handshake
  c.write("data_on, 8000");
  
  udp = new UDP(this, 8000);
  
  udp.listen( true );
  
  CX=width/2 ; CY=height/2;
  for(int i=0;i<numstars;i++) {    
    s[i]= new Star();    
    s[i].SetPosition();  
  }
}

void draw() 
{
  background(0,0,0);
  drawStarField();
  
  fill(255);
  rect(560, 490, 200, 100);
  
  rect(860, 490, 200, 100);
  
  rect(1160, 490, 200, 100);
  // Receive data from server
  
  
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


