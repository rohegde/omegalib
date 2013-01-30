@vertexShader

uniform vec2 unif_TextureTiling;
varying vec2 var_TexCoord;
attribute vec3 deformation;
varying vec3 def;

///////////////////////////////////////////////////////////////////////////////////////////////////
void setupSurfaceData(vec4 eyeSpacePosition)
{
	var_TexCoord = gl_MultiTexCoord0.xy * unif_TextureTiling;
	def =deformation;
}