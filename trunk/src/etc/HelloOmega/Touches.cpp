#include "Touches.h"

Touches::Touches(){
		timeStamp = 0;
        finger = -1;
        xPos = 0;
        yPos = 0;
        //this.intensity = intensity;
        type = -1;
        xWidth = 0;
        yWidth = 0;
}// CTOR

Touches::Touches(int typeID, long ts, float x, float y, float xW, float yW, int ID){
		timeStamp = ts;
        finger = ID;
        xPos = x;
        yPos = y;
        //this.intensity = intensity;
        type = typeID;
        xWidth = xW;
        yWidth = yW;
}// CTOR

void Touches::updateTouch(Touches newTouch)
{
        timeStamp = newTouch.timeStamp;
        xPos = newTouch.xPos;
        yPos = newTouch.yPos;
        type = newTouch.type;
        xWidth = newTouch.xWidth;
        yWidth = newTouch.yWidth;
}// updateTouch

long Touches::getTimeStamp()
{
	return timeStamp;
}// getTimeStamp

int Touches::getFinger()
{
	return finger;
}// getFinger

float Touches::getXPos()
{
	return xPos;
}// getXPos

float Touches::getYPos()
{
	return yPos;
}// getYPos

float Touches::getIntensity()
{
	return intensity;
}// getIntensity

bool Touches::isDown()
{
	if (type == 0)
		return true;
	else
		return false;
}// isDown

bool Touches::isMove()
{
	if (type == 1)
		return true;
	else
		return false;
}// isMove

bool Touches::isUp()
{
	if (type == 2)
		return true;
	else
		return false;
}// isUp
