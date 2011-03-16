class Button {
  
  public float x;
  public float y;
  public float width;
  public float height;
  
  public String text;
  
  Button(float x, float y, float width, float height, String text) {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
    this.text = text;
  }
  
  void draw()  {
    stroke(255);
    fill(0);
    rect(x, y, width, height);
    rectMode( CENTER );
  }
  
  void setx(float x) {
    
  }
  
  void sety(float y) {
  }
  
}
