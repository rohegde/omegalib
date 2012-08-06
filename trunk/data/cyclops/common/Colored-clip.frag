@surfaceShader 

uniform float unif_Shininess;
uniform float unif_Gloss;

uniform float unif_ClipX;

varying float var_CurrentX;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
    sd.albedo = gl_Color;
	if(var_CurrentX > unif_ClipX) discard;
	sd.emissive = vec4(0, 0, 0, 0);
	sd.shininess = unif_Shininess;
	sd.gloss = unif_Gloss;
	
	return sd;
}
