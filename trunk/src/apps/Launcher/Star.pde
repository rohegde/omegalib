class Star {
  float x=0, y=0, z=0, sx=0, sy=0;  
  
  Star() {    
     
  }  


  void SetPosition() {
    z= (float) random(200,255);
    x= (float) random(-1000,1000);    
    y= (float) random(-1000,1000); 
  }
  
  void draw() {    
    if ( z < SPEED ) {	
      this.SetPosition();    
    }    
    z -= SPEED;    
    sx = (x*SPREAD) / (z) + CX;    
    sy = (y*SPREAD) / (4+z) + CY;    
  
    if (sx<0 | sx>width) {	
      this.SetPosition();    
    }    
    if (sy<0 | sy>height) {
      this.SetPosition();    
    }    
  
    //fill(color(255 - (int) z,255 - (int) z,255 - (int) z));    
    //fill(color((int) z - 255,(int) z - 255,(int) z - 255));    
    //fill(1.0, 0, 0);
    //gl.glColor3f(1.0, 0.0, 0.0);
    rect( (int) sx,(int) sy,1,2);  
  }
}
