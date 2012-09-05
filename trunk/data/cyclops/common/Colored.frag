@surfaceShader 

uniform float unif_Shininess;
uniform float unif_Gloss;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
    sd.albedo.rgb = gl_Color.rgb; 
	sd.albedo.a = 1.0;
	sd.emissive = vec4(0, 0, 0, 0);
	sd.shininess = unif_Shininess;
	sd.gloss = unif_Gloss;
	
	return sd;
}
