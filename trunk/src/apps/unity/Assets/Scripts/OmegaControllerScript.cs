/**
 * ---------------------------------------------
 * OmegaControllerScript.cs
 * Description: Manages InputServiceScript controller data.
 * 		For First Person Controller prefab: Can be used to replace FPSInputController.js and MouseLook.js.
 * 
 * Copyright 2010, Electronic Visualization Laboratory, University of Illinois at Chicago.
 * Author(s): Arthur Nishimoto
 * ---------------------------------------------
 */
 
using UnityEngine;
using System.Collections;

public class OmegaControllerScript : MonoBehaviour {
	public Vector3 movementVector;
	public Vector3 rotationVector;
	public Vector3 tiltVector;
	
	public float SensitivityX = 2.0f;
	public float MinSensitivityX = 0.1f;
	
	public float SensitivityY = 2.0f;
	public float MinSensitivityY = 0.1f;
	
	public float MinPitch = 15f;
	public float MinRoll = 15f;
	
	public float MinPitchSensitivity = 1f;
	public float MinRollSensitivity = 1f;
	
	private float lastPitch = 0;
	private float lastRoll = 0;
	
	const int RAW = 0;
	const int CONTROLLED = 1;
	const int ANALOG_STICK = 2;
	int TILT_MODE = CONTROLLED;
	
	int[] heldButtons;
	
	// Use this for initialization
	void Start () {
		movementVector = new Vector3();
		rotationVector = new Vector3();
		tiltVector = new Vector3();
		heldButtons = new int[16];
	}
	
	// Update is called once per frame
	void Update () {
		float playerHeadOffset = GameObject.FindWithTag("Player").GetComponent<CharacterController>().height/2 * 0.8f;
		Camera.main.transform.position = GameObject.FindWithTag("Player").transform.position + new Vector3(0, playerHeadOffset, 0);
		Camera.main.transform.eulerAngles = transform.eulerAngles;
		
		// Lock player from rotating down/up (camera/head should do this)
		float rotationX = 0;
		if( TILT_MODE == ANALOG_STICK )
			rotationX = transform.localEulerAngles.x;
		float rotationY = transform.localEulerAngles.y;
		transform.localEulerAngles = new Vector3(rotationX, rotationY, 0);
		
		// RotateX/Y
		transform.Rotate(rotationVector+tiltVector);
		Camera.main.transform.Rotate(rotationVector+tiltVector);
	}
	
	public void MoveVector( float[] value ){
		movementVector = new Vector3( value[0], 0, -value[1] );
		//transform.Translate(new Vector3( value[0], 0, -value[1] ));
	}
	
	public void RotateVector( float[] value ){
		if( Mathf.Abs(value[0]) > MinSensitivityY )
			rotationVector = new Vector3( 0, value[0] * SensitivityY, 0 );
		else
			rotationVector = Vector3.zero;
		
		if( TILT_MODE == ANALOG_STICK ){
			if( Mathf.Abs(value[1]) > MinSensitivityX )
				tiltVector = new Vector3( value[1] * SensitivityX, 0, 0);
			else
				tiltVector = Vector3.zero;
		}
		//Camera.main.transform.Rotate( rotationVector * Time.deltaTime );
	}
	
	public void Buttons( int[] buttons ){
		// PS3 Cross - Reset camera
		if( buttons[0] == 1 && heldButtons[0] == 0 ){ // On down
			
			heldButtons[0] = 1; // Mark button as held
		} else if( buttons[0] == 0 && heldButtons[0] == 1 ) { // On release
			heldButtons[0] = 0; // Release hold state
		}
		
		// PS3 L1 - Reset camera
		if( buttons[4] == 1 && heldButtons[4] == 0 ){ // On down
			transform.localEulerAngles = new Vector3(0, 0, 0); 
			heldButtons[4] = 1;
		} else if( buttons[4] == 0 && heldButtons[4] == 1 ) { // On release
			heldButtons[4] = 0;
		}
		
		// PS3 R1 - Toggle tilt mode
		if( buttons[5] == 1 && heldButtons[5] == 0 ){ // On down
			if( TILT_MODE < 2 )
				TILT_MODE++;
			else
				TILT_MODE = 0;
			heldButtons[5] = 1;
		} else if( buttons[5] == 0 && heldButtons[5] == 1 ) { // On release
			heldButtons[5] = 0;
		}
		
	}
	
	public void TiltVector( float roll, float pitch ){
		roll *= 90.0f;
		pitch *= -90.0f;
		
		//Debug.Log( pitch + " " + roll );
		
		switch( TILT_MODE ){
			case(CONTROLLED):
				// If the new pitch/roll is below the minimum and the current pitch is greater
				// that min pitch, reset to zero.
				// We check if last < min so a level controller 
				if( lastPitch < MinPitch && Mathf.Abs(pitch) < MinPitch ){
					pitch = 0;
					lastPitch = 0;
				}
				
				if( lastRoll < MinRoll && Mathf.Abs(roll) < MinRoll ){
					roll = 0;
					lastRoll = 0;
				}
				
				float pitchDiff = Mathf.Abs(lastPitch) - Mathf.Abs(pitch);
				float rollDiff = Mathf.Abs(lastRoll) - Mathf.Abs(roll);
			
				//float newPitch = 0;
				//float newRoll = 0;
				if( Mathf.Abs(pitchDiff) > MinPitchSensitivity ){
					lastPitch = pitch;
				}
				
				if( Mathf.Abs(rollDiff) > MinRollSensitivity ){
					lastRoll = roll;
				}
				
				tiltVector= new Vector3( lastPitch, 0, lastRoll );
				break;
			case(RAW):
				tiltVector= new Vector3( pitch, 0, roll );
				break;
			default:
				break;
		}
	}
	
	public void ButtonPressed( int ID ){
		switch(ID){
			case(1): renderer.material.color = Color.blue; break;
			case(2): renderer.material.color = Color.red; break;
			case(3): renderer.material.color = Color.magenta; break;
			case(4): renderer.material.color = Color.green; break;
			default: renderer.material.color = Color.white; break;
		}
	}
}
