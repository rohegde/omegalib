class Button {
  
  public float x;
  public float y;
  public float width;
  public float height;
  
  PFont font;
  
  public String text;
  
  Button(float x, float y, float width, float height, String text) {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
    this.text = text;
    
    font = createFont("Helvetica", 32);
    
  }
  
  void draw()  {
    stroke(255);
    fill(0);
    rect(x, y, width, height);
    rectMode( CENTER );
    
    textFont(font);
    fill(255);
    textAlign( CENTER );
    text(this.text, x, y);
  }
  
  void setx(float x) {
    
  }
  
  void sety(float y) {
  }
  
}
