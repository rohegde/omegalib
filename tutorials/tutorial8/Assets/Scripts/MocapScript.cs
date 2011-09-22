using UnityEngine;
using System.Collections;

public class MocapScript : MonoBehaviour {
	public int trackedObjectID;
	public Vector3 offsetFromOrigin = new Vector3( 0, -1.5f, -1.7f );
	
	bool moving = false;
	public bool stopJitter = false;
	float idleStartTime;
	float idleTime = 0;
	
	// Data as given by tracker
	Vector3 rawMocapPosition = new Vector3();
	Quaternion rawMocapOrientation = new Quaternion();
	
	// Data translated into Unity coordinate system
	Vector3 mocapPosition = new Vector3();
	Quaternion mocapOrientation = new Quaternion();
	
	void Start () {
		// Reserved for derived classes
	}

	void Update () {
		// Reserved for derived classes
		
		// Basic drag-and-drop for tracked objects
		if( Vector3.Distance( getPosition(), gameObject.transform.position) > 0.04 )
		{
			moving = true;
			idleTime = 0;
		} else {
			idleTime++;
			if ( idleTime > 10 ){
				moving = false;
			}
		}
	
		if( moving || !stopJitter ){
			gameObject.transform.position = Vector3.Lerp( gameObject.transform.position, getPosition(), Time.deltaTime * 6 );
		} else {
		}
	}
		
	public void UpdateMocapPosition( string[] words ){
		int ID = int.Parse(words[1]);
		if( ID == trackedObjectID ){
			float x = float.Parse(words[2]);
			float y = float.Parse(words[3]);
			float z = float.Parse(words[4]);
			
			float xR = float.Parse(words[2]);
			float yR = float.Parse(words[3]);
			float zR = float.Parse(words[4]);
			float wR = float.Parse(words[5]);
			
			rawMocapPosition = new Vector3( x, y, z );
			rawMocapOrientation = new Quaternion( xR, yR, zR, wR );
			
			mocapPosition = new Vector3( x, y, -z );
			mocapOrientation = new Quaternion( xR, yR, zR, -wR );
		}
	}
	
	// Getters/Setters ------------------------------------------------------------
	public Vector3 getRawPosition(){
		return rawMocapPosition;
	}
	
	public Quaternion getRawOrientation(){
		return rawMocapOrientation;
	}
	
	public Vector3 getPosition(){
		return mocapPosition + offsetFromOrigin;
	}
	
	public Quaternion getOrientation(){
		return mocapOrientation;
	}
}
