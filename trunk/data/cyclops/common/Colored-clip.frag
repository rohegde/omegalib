@surfaceShader 

uniform float unif_Shininess;
uniform float unif_Gloss;

uniform float unif_ClipX;
uniform bool unif_Trans;

varying float var_CurrentX;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
	
	sd.emissive.rgb = gl_Color.rgb;	
	if(unif_Trans) sd.emissive.a = gl_Color.r;

	sd.albedo.a=1.0;
	
	if(var_CurrentX > unif_ClipX) discard;
	

	sd.shininess = unif_Shininess;
	sd.gloss = unif_Gloss;
	
	return sd;
}
