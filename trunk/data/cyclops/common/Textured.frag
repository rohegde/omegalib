@use computeShadow
@use computeStandardShading
@use computeEnvMap

///////////////////////////////////////////////////////////////////////////////////////////////////
uniform sampler2D unif_ColorMap;
varying vec2 var_TexCoord;

///////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	float shadow = computeShadow();
	
	vec4 diffuseTextureColor = texture2D(unif_ColorMap, var_TexCoord);
	
    gl_FragColor =  diffuseTextureColor * (envMapAmbientContribution() * lightAmbientContribution() + lightDiffuseContribution() * shadow); 
}
