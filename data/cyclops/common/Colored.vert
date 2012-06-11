@use setupShadow
@use setupStandardShading
@use setupEnvMap
	                     
///////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	gl_Position = ftransform();
	vec4 eyeSpacePosition = gl_ModelViewMatrix * gl_Vertex;
		
	setupShadow(eyeSpacePosition);
	setupStandardShading(eyeSpacePosition);
	setupEnvMap(eyeSpacePosition);
	
	gl_FrontColor = gl_Color;
}
