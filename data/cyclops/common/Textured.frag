@use computeShadow
@use computeStandardShading

///////////////////////////////////////////////////////////////////////////////////////////////////
uniform sampler2D unif_ColorMap;
varying vec2 var_TexCoord;

///////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	float shadow = computeShadow();
	vec4 lightColor = computeStandardShading();
	
	vec4 diffuseTextureColor = texture2D(unif_ColorMap, var_TexCoord);
	
    gl_FragColor = lightColor * diffuseTextureColor * shadow; 
}
