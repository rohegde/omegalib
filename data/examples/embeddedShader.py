# basic omegalib script: just display a textured spinning cube.
from omega import *
from cyclops import *

scene = getSceneManager()
scene.createProgramFromString("glow", 
# Vertex shader
'''
	varying vec2 var_TexCoord;
	varying vec3 var_Normal;
	varying vec3 var_EyeVector;

	void main(void)
	{
		gl_Position = ftransform();
		vec4 eyeSpacePosition = gl_ModelViewMatrix * gl_Vertex;
		
		var_TexCoord = gl_MultiTexCoord0.xy;
		
		var_EyeVector = -eyeSpacePosition.xyz;
		var_Normal = gl_NormalMatrix * gl_Normal;
		
		gl_FrontColor = gl_Color;
	}
''',
# Fragment shader
'''
	varying vec2 var_TexCoord;

	void main (void)
	{
		float vx = pow(abs((var_TexCoord.x - 0.5) * 2), 10);
		float vy = pow(abs((var_TexCoord.y - 0.5) * 2), 10);

		gl_FragColor.rgb = gl_Color.rgb;	
		gl_FragColor.a = (vx + vy);
	}
''')

box = BoxShape.create(0.8, 0.8, 0.8)
box.setPosition(Vector3(0, 2, -3))

# Apply an emissive textured effect (no lighting)
box.setEffect("glow -d red -t")

# Spin the box!
def onUpdate(frame, t, dt):
	box.pitch(dt)
	box.yaw(dt / 3)
setUpdateFunction(onUpdate)
