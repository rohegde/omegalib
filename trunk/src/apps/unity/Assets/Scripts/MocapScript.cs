using UnityEngine;
using System.Collections;

public class MocapScript : MonoBehaviour {
	public int myID;
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
			gameObject.transform.position = new Vector3(x,y,z);
		}
	}
}
