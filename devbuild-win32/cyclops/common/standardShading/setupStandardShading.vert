varying vec3 var_Normal;
varying vec3 var_LightDir;
varying vec3 var_HalfVector;
varying float var_LightDist;

///////////////////////////////////////////////////////////////////////////////////////////////////
void setupStandardShading(vec4 eyeSpacePosition)
{
	var_Normal = gl_NormalMatrix * gl_Normal;
	
	vec3 aux = vec3(gl_LightSource[0].position.xyz - eyeSpacePosition.xyz);
	var_LightDir = normalize(aux);
	var_LightDist = length(aux);
	
	var_HalfVector = normalize(gl_LightSource[0].halfVector.xyz);
}
