@use computeShadow
@use computeStandardShading
@use computeEnvMap

///////////////////////////////////////////////////////////////////////////////////////////////////
void main (void)
{
	float shadow = computeShadow();
    gl_FragColor =  gl_Color * (envMapAmbientContribution() * lightAmbientContribution() + lightDiffuseContribution() * shadow); 
}
