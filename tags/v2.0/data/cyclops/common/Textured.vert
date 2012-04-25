@use setupShadow
@use setupStandardShading

///////////////////////////////////////////////////////////////////////////////////////////////////
uniform vec2 unif_TextureTiling;
varying vec2 var_TexCoord;

///////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	gl_Position = ftransform();
	var_TexCoord = gl_MultiTexCoord0.xy * unif_TextureTiling;
	vec4 eyeSpacePosition = gl_ModelViewMatrix * gl_Vertex;
		
	setupShadow(eyeSpacePosition);
	setupStandardShading(eyeSpacePosition);
}
