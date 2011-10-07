using UnityEngine;
using System.Collections;

public class WiimoteControllerScript : OmegaControllerScript {
	public Vector3 analogStick;
	
	public Vector3 accelerometerVal0;
	public float accelerometerMagnitude;
	
	public Vector3 accelerometerVal1;
	
	public bool motionPlusCalibrationSet = false;
	public Vector3 motionPlusCalibration;
	public Vector3 motionPlus;
	public float motionPlusDifference;
	public float movementThreshold = 0.2f;
	
	public int A = 0;
	public int B = 0;
	public int Plus = 0;
	public int Home = 0;
	public int Minus = 0;
	public int One = 0;
	public int Two = 0;
	public int Up = 0;
	public int Down = 0;
	public int Left = 0;
	public int Right = 0;
	public int C = 0;
	public int Z = 0;
	
	// Use this for initialization
	void Start () {
		analogStick0Deadzone = new Vector3(10,10,10);
		
	}
	
	// Update is called once per frame
	void Update () {
		analogStick = getAnalogStick0();
		accelerometerVal0 = getAccelerometer0();
		accelerometerVal1 = getAccelerometer1();
		
		accelerometerMagnitude = accelerometerVal0.magnitude;
		
		if( getAccelerometer2() != Vector3.zero && !motionPlusCalibrationSet ){
			motionPlusCalibration = getAccelerometer2();
			motionPlusCalibrationSet = true;
		}
		motionPlus = getAccelerometer2() - motionPlusCalibration;
		
		motionPlusDifference = Vector3.Magnitude(getAccelerometer2() - motionPlusCalibration);
		
		if( motionPlusDifference >= movementThreshold ){
			transform.Rotate(motionPlus.y, -motionPlus.x, motionPlus.z);
			//transform.Translate(motionPlus.z, 0, 0);
		}
		
		// Testing
		transform.Translate(0, 0, analogStick.y * Time.deltaTime);
		transform.Rotate(0, analogStick.x * Time.deltaTime * 50, 0);
		
		A = getButton( Button.A );
		B = getButton( Button.B );
		Plus = getButton( Button.Plus );
		Minus = getButton( Button.Minus );
		Home = getButton( Button.Home );
		One = getButton( Button.One );
		Two = getButton( Button.Two );
		C = getButton( Button.C );
		Z = getButton( Button.Z );
		Up = getButton( Button.Up );
		Down = getButton( Button.Down );
		Left = getButton( Button.Left );
		Right = getButton( Button.Right );
		
	}
}
