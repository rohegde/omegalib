using UnityEngine;
using System.Collections;

public class MocapScript : MonoBehaviour {
	public int myID;
	public Vector3 tempVar = new Vector3(100,100,100);
	bool moving = false;
	public bool stopJitter = true;
	float idleStartTime;
	float idleTime = 0;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	//public void UpdateMocapPosition( float x, float y, float z){
	//	gameObject.transform.position = new Vector3(x,y,z);
	//}
	
	public void UpdateMocapPosition( string[] words ){
		int ID = int.Parse(words[1]);
		if( ID == myID ){
			float x = float.Parse(words[2]);
			float y = float.Parse(words[3]);
			float z = - float.Parse(words[4]);
			tempVar = new Vector3(x,y,z);
			//print("X: " + x + " Y: " + y + " Z: " + z);
			//if(x > 5 + tempVar[0] || x < 5 + tempVar[0] || y > 5 + tempVar[1] || y < 5 + tempVar[1] || z > 5 + tempVar[2] || z < 5 + tempVar[2])
			if( Vector3.Distance( tempVar, gameObject.transform.position) > 0.04 )
			{
				//gameObject.transform.position = new Vector3(x,y,z);
				moving = true;
				idleTime = 0;
			} else {
				idleTime++;
				if ( idleTime > 10 ){
					moving = false;
				}
			}
	
			if( moving || !stopJitter ){
				gameObject.transform.position = Vector3.Lerp( gameObject.transform.position, tempVar, Time.deltaTime * 6 );
			} else {
			}
		}
	}
}
