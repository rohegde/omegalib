using UnityEngine;
using System.Collections;

public class MocapExampleScript : MocapScript {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		
		// Set this object's position to the tracked object's position by using Lerp to interpolate/smooth out the movement.
		gameObject.transform.position = Vector3.Lerp( gameObject.transform.position, getPosition(), Time.deltaTime * 6 );
		
		// Set object's orientation to the tracked object's orientation
		// TODO: 9-9-2011: Not working correctly - probably not converted to Unity's coordinate system properly.
		//gameObject.transform.rotation = getOrientation();
	}
}
