#ifndef TOUCHES_H_
#define TOUCHES_H_

class Touches{
private:
	int finger;
    int type;
        // DOWN = 0
        // MOVE = 1
        // UP   = 2
    long timeStamp;
    float xPos, yPos, intensity;
    float xWidth, yWidth;
public:
	Touches();
	Touches(int type, long timeStamp, float xPos, float yPos, float xWidth, float yWidth, int finger);
	
	void updateTouch(Touches);
	long getTimeStamp();
	int getFinger();
	float getXPos();
	float getYPos();
	float getIntensity();
	bool isDown();
	bool isMove();
	bool isUp();
	//float[] getFingerSize();
}; // class

#endif // end of header