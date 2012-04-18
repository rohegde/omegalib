@use computeShadow
@use computeStandardShading

///////////////////////////////////////////////////////////////////////////////////////////////////
void main (void)
{
	float shadow = computeShadow();
    gl_FragColor =  gl_Color * (lightAmbientContribution() + lightDiffuseContribution() * shadow); 
}
