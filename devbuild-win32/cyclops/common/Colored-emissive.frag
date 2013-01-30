@surfaceShader

uniform float unif_Shininess;
uniform float unif_Gloss;

varying vec3 var_Normal;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
    sd.albedo = vec4(0, 0, 0, 0); 
	sd.emissive = gl_Color;
	sd.shininess = 0;
	sd.gloss = 0;
	sd.normal = normalize(var_Normal);
	
	return sd;
}
