///////////////////////////////////////////////////////////////////////////////////////////////////
uniform samplerCube unif_CubeMap;
varying vec3 var_ReflectDir;

///////////////////////////////////////////////////////////////////////////////////////////////////
vec4 getEnvMapColor(void)
{
	//Cubemap
	return vec4(textureCube(unif_CubeMap, var_ReflectDir));
}