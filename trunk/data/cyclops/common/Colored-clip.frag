@surfaceShader 

uniform float unif_Shininess;
uniform float unif_Gloss;

uniform float unif_ClipX;
uniform boolean unif_Trans;

varying float var_CurrentX;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
	float red;
	red=gl_Color.r;
    sd.albedo = gl_Color;
	
	if(unif_Trans) sd.albedo.a=red;
	
	if(var_CurrentX > unif_ClipX) discard;
	
	sd.emissive = vec4(0, 0, 0, 0);
	sd.shininess = unif_Shininess;
	sd.gloss = unif_Gloss;
	
	return sd;
}
