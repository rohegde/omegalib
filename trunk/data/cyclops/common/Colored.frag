@use computeShadow
@use computeStandardShading

///////////////////////////////////////////////////////////////////////////////////////////////////
void main (void)
{
	float shadow = computeShadow();
	vec4 lightColor = computeStandardShading();
    gl_FragColor =  gl_FrontMaterial.ambient + (lightColor * gl_Color * shadow); 
}
