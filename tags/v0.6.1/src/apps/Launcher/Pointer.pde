class Pointer {
  public float x;
  public float y;
  public boolean onScreen;
  
  Pointer() {
    onScreen = false;
  }
  
  void draw() {
    
    if( onScreen ) {
      fill(255, 0, 0);
      
      rect(x, y, 30, 30);
    }
    
  }
}
