using UnityEngine;
using System.Collections;

public class MocapScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	public void UpdateMocapPosition( float x, float y, float z){
		gameObject.transform.position = new Vector3(x,y,z);
	}
}
