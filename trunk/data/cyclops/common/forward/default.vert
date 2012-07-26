@vsinclude shadowMap
@vsinclude envMap

varying vec3 var_Normal;
varying vec3 var_EyeVector;

///////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	gl_Position = ftransform();
	vec4 eyeSpacePosition = gl_ModelViewMatrix * gl_Vertex;
	
	setupShadowMap(eyeSpacePosition);
	setupEnvMap(eyeSpacePosition.xyz);
	
	var_EyeVector = -eyeSpacePosition;
	var_Normal = gl_NormalMatrix * gl_Normal;
	
	gl_FrontColor = gl_Color;
}
