@use setupShadow
@use setupStandardShading
	                     
///////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	gl_Position = ftransform();
	vec4 eyeSpacePosition = gl_ModelViewMatrix * gl_Vertex;
		
	setupShadow(eyeSpacePosition);
	setupStandardShading(eyeSpacePosition);
	
	gl_FrontColor = gl_Color;
}
